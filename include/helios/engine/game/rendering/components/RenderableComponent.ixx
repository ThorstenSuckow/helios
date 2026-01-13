/**
 * @file RenderableComponent.ixx
 * @brief Component for associating a renderable object with an entity.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.game.rendering.components.RenderableComponent;

import helios.engine.game.CloneableComponent;
import helios.engine.game.GameObject;

import helios.rendering.Renderable;
import helios.math.types;

import helios.engine.game.model.components.ModelAabbComponent;

export namespace helios::engine::game::rendering::components {

    /**
     * @brief Component that holds a shared pointer to a Renderable resource.
     *
     * @details
     * This component links a GameObject to a visual representation (Renderable).
     * When attached, it automatically extracts the AABB from the renderable's mesh
     * and populates a ModelAabbComponent on the same GameObject.
     */
    class RenderableComponent : public helios::engine::game::CloneableComponent<RenderableComponent> {

        /**
         * @brief Shared pointer to the renderable resource.
         */
        std::shared_ptr<helios::rendering::Renderable> renderable_;

        /**
         * @brief The initial AABB of the model associated with the renderable.
         */
       helios::math::aabbf aabb_{};

    public:

        /**
         * @brief Constructs a RenderableComponent.
         *
         * @param renderable Shared pointer to the Renderable. Must not be nullptr.
         */
        explicit RenderableComponent(std::shared_ptr<helios::rendering::Renderable> renderable) :
        renderable_(std::move(renderable)) {

            assert(renderable_ != nullptr && "renderable must not be nullptr");
            aabb_ = renderable_->renderPrototype()
                  ? renderable_->renderPrototype()->mesh().aabb()
                  : helios::math::aabbf{};

        }

        /**
         * @brief Copy Constructor
         */
        explicit RenderableComponent(const RenderableComponent& other)  :
        renderable_(std::move(other.renderable_)), aabb_(other.aabb_) {}


        /**
         * @brief Called when the component is attached to a GameObject.
         *
         * @details
         * Automatically adds or retrieves a ModelAabbComponent on the GameObject
         * and initializes it with the captured AABB from the Renderable.
         *
         * @param gameObject Pointer to the parent GameObject.
         */
        void onAttach(helios::engine::game::GameObject* gameObject) noexcept override {
            Component::onAttach(gameObject);

            auto& msc = gameObject->getOrAdd<helios::engine::game::model::components::ModelAabbComponent>();

            msc.setAabb(aabb_);
        }

        /**
         * @brief Retrieves the stored Renderable.
         *
         * @return Shared pointer to the Renderable.
         */
        [[nodiscard]] std::shared_ptr<helios::rendering::Renderable> renderable() const noexcept {
            return renderable_;
        }

    };


}