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

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.math;


export namespace helios::engine::modules::ui::transform::systems {


    /**
     * @brief System for computing UI element screen positions.
     *
     * Processes entities with UiTransformComponent to compute their final
     * screen positions based on anchor points, pivot points, margins, and
     * current viewport dimensions.
     */
    class UiTransformSystem : public helios::engine::ecs::System {
        /**
         * @brief Adjusts position based on pivot point and element size.
         *
         * @param unanchored The position before pivot adjustment.
         * @param size The size of the element.
         * @param pivot The pivot point to apply.
         *
         * @return The adjusted position.
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
            }

            assert(false && "Unreachable!");
            std::unreachable();
        }

    public:

        /**
         * @brief Updates UI element positions based on viewport dimensions.
         *
         * Queries entities with UiTransformComponent and computes their screen
         * positions based on anchor, pivot, margins, and current viewport bounds.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, tc, tsc, ctc, mbc, active] : gameWorld_->view<
                helios::engine::modules::ui::transform::components::UiTransformComponent,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent,
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
                helios::engine::modules::rendering::model::components::ModelAabbComponent,
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

                    const auto viewportWidth = snapshot.absoluteBounds[2];
                    const auto viewportHeight = snapshot.absoluteBounds[3];

                    const auto margins = tc->margins();

                    switch (tc->anchor()) {

                        case helios::engine::modules::ui::layout::Anchor::Center:
                            tsc->setTranslation({viewportWidth / 2.0f, viewportHeight / 2.0f, 0.0f});
                            break;

                        case helios::engine::modules::ui::layout::Anchor::TopRight:
                            auto anchored = helios::math::vec3f(
                                viewportWidth - margins[1],
                                viewportHeight - margins[0],
                                0.0f
                            );

                            anchored = anchor(anchored, mbc->aabb().size(), pivot);

                            tsc->setTranslation(anchored);
                            break;

                    }
                }
            }
        }
    };
}


