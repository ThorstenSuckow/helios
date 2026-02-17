/**
 * @file UiTransformSystem.ixx
 * @brief System for computing UI element screen positions.
 */
module;

#include <cassert>
#include <utility>

export module helios.engine.modules.ui.transform.systems.UiTransformSystem;

import helios.engine.ecs.System;

import helios.engine.modules.ui.transform.components.UiTransformComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;
import helios.engine.modules.ui.layout.Anchor;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

import helios.engine.modules.scene.components.SceneNodeComponent;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.math;

import helios.engine.ecs.components.HierarchyComponent;

export namespace helios::engine::modules::ui::transform::systems {


    /**
     * @brief Computes screen positions for UI elements based on layout configuration.
     *
     * @details This system processes entities with UiTransformComponent and computes
     * their final screen positions. The positioning algorithm considers:
     *
     * - **Anchor:** Where the element attaches to its parent (Center, TopLeft, etc.)
     * - **Pivot:** The element's own reference point for positioning
     * - **Offsets:** Margins from the anchor point (top, right, bottom, left)
     * - **Viewport:** Current viewport dimensions for root-level elements
     * - **Parent bounds:** For nested UI elements via HierarchyComponent
     *
     * ## Hierarchy Support
     *
     * For elements with a HierarchyComponent, the system uses the parent entity's
     * AABB and transform to compute relative positioning. Root elements (those
     * attached to the scene root) use viewport dimensions as their parent bounds.
     *
     * ## Required Components
     *
     * | Component | Purpose |
     * |-----------|---------|
     * | `UiTransformComponent` | Layout configuration (anchor, pivot, offsets) |
     * | `TranslationStateComponent` | Receives computed position |
     * | `ComposeTransformComponent` | Transform composition |
     * | `ModelAabbComponent` | Element bounds for size calculations |
     * | `SceneNodeComponent` | Scene graph integration |
     * | `Active` | Lifecycle tag |
     *
     * @see UiTransformComponent
     * @see Anchor
     * @see HierarchyComponent
     */
    class UiTransformSystem : public helios::engine::ecs::System {

        /**
         * @brief Adjusts position based on pivot point and element size.
         *
         * @details Transforms a position from anchor-relative to pivot-relative
         * coordinates. The pivot determines which point of the element is placed
         * at the anchor position.
         *
         * @param unanchored The position before pivot adjustment.
         * @param size The size of the element.
         * @param pivot The pivot point to apply.
         *
         * @return The adjusted position accounting for pivot offset.
         */
        helios::math::vec3f anchor(
                const helios::math::vec3f unanchored,
                helios::math::vec3f size,
                const helios::engine::modules::ui::layout::Anchor pivot) {

            switch (pivot) {

                case helios::engine::modules::ui::layout::Anchor::Center:
                    return unanchored - size * 0.5f;

                case helios::engine::modules::ui::layout::Anchor::TopRight:
                    return unanchored - size;

                case helios::engine::modules::ui::layout::Anchor::TopLeft:
                    return unanchored.withY(unanchored[1] - size[1]);

                case helios::engine::modules::ui::layout::Anchor::BottomLeft:
                    return unanchored;
            }

            assert(false && "Unreachable!");
            std::unreachable();
        }

    public:

        /**
         * @brief Computes and applies screen positions for all UI elements.
         *
         * @details Iterates over all entities with UI layout components and computes
         * their final screen positions. For each entity:
         *
         * 1. Finds the matching viewport snapshot by viewportId
         * 2. Determines parent bounds (viewport for root, parent AABB otherwise)
         * 3. Computes anchor position based on anchor type and offsets
         * 4. Adjusts for pivot point
         * 5. Updates TranslationStateComponent with final position
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, tc, tsc, ctc, mbc, snc, active] : gameWorld_->view<
                helios::engine::modules::ui::transform::components::UiTransformComponent,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent,
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
                helios::engine::modules::rendering::model::components::ModelAabbComponent,
                helios::engine::modules::scene::components::SceneNodeComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                if (!tc->viewportId()) {
                    continue;
                }

                for (const auto& snapshot : updateContext.viewportSnapshots()) {
                    if (snapshot.viewportId != tc->viewportId()) {
                        continue;
                    }

                    auto pivot = tc->pivot();

                    auto sceneNode = snc->sceneNode();

                    float viewportWidth;
                    float viewportHeight;

                    auto uiRect   = helios::math::vec4f(
                        mbc->aabb().min()[0], mbc->aabb().min()[1],
                        mbc->aabb().size()[0], mbc->aabb().size()[1]
                    );
                    auto worldRect = helios::math::vec4f(
                        uiRect[0] + uiRect[0] / 2.0f,
                        uiRect[1] + uiRect[1] / 2.0f,
                        uiRect[2], uiRect[3]
                    );


                    auto parentUiRect = helios::math::vec4f{};
                    auto parentWorldRect = helios::math::vec4f{};

                    const auto offsets = tc->offsets();

                    if (sceneNode->parent()->isRoot()) {
                        viewportWidth = snapshot.absoluteBounds[2];
                        viewportHeight = snapshot.absoluteBounds[3];

                        parentUiRect = helios::math::vec4f(0, 0, viewportWidth, viewportHeight);

                        parentWorldRect = helios::math::vec4f(
                            viewportWidth / 2.0f,
                            viewportHeight /2.0f,
                            viewportWidth,
                            viewportHeight
                        );

                    } else {

                        auto* hc = entity.get<helios::engine::ecs::components::HierarchyComponent>();

                        if (!hc || !hc->parent()) {
                            continue;
                        }

                        // we rely on the parent entity so we do not have to wait for the SceneGraph sync
                        if (auto parentGo = gameWorld_->find(hc->parent().value())) {
                            auto* pmaabbcc = parentGo->get<rendering::model::components::ModelAabbComponent>();
                            auto* pctc = parentGo->get<spatial::transform::components::ComposeTransformComponent>();

                            auto size = pmaabbcc->aabb().size() * pctc->localScaling();

                            parentUiRect = helios::math::vec4f(
                                -(size[0] * 0.5f),
                                -(size[1] * 0.5f),
                                size[0],
                                size[1]
                            );
                        }
                    }


                    switch (tc->anchor()) {

                        case helios::engine::modules::ui::layout::Anchor::Center: {
                            auto anchored = helios::math::vec3f(
                                parentUiRect[0] + parentUiRect[2]/2.0f - offsets[1] ,
                                parentUiRect[1] + parentUiRect[3]/2.0f - offsets[0], 0.0f
                            );
                            anchored = anchor(anchored, mbc->aabb().size(), pivot);
                            tsc->setTranslation(anchored);
                        }
                            break;

                        case helios::engine::modules::ui::layout::Anchor::TopRight: {
                            auto anchored = helios::math::vec3f(
                                parentUiRect[0] + parentUiRect[2] - offsets[1],
                                parentUiRect[1] + parentUiRect[3] - offsets[0],
                                0.0f
                            );
                            anchored = anchor(anchored, mbc->aabb().size(), pivot);
                            tsc->setTranslation(anchored);
                        }
                            break;

                        case helios::engine::modules::ui::layout::Anchor::TopLeft: {
                            auto anchored = helios::math::vec3f(
                               parentUiRect[0] + offsets[3],
                               parentUiRect[1] + parentUiRect[3] - offsets[0],
                               0.0f
                           );
                            anchored = anchor(anchored, mbc->aabb().size(), pivot);
                            tsc->setTranslation(anchored);
                        }
                            break;

                        case helios::engine::modules::ui::layout::Anchor::BottomLeft: {
                            auto anchored = helios::math::vec3f(
                               parentUiRect[0] + offsets[3],
                               parentUiRect[1] + offsets[2],
                               0.0f
                            );
                            anchored = anchor(anchored, mbc->aabb().size(), pivot);
                            tsc->setTranslation(anchored);
                        }
                            break;

                    }
                }
            }
        }
    };
}


