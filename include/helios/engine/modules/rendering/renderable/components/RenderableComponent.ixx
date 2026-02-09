/**
 * @file RenderableComponent.ixx
 * @brief Component for associating a renderable object with an entity.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.modules.rendering.renderable.components.RenderableComponent;


import helios.engine.ecs.GameObject;

import helios.rendering.Renderable;
import helios.math.types;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

export namespace helios::engine::modules::rendering::renderable::components {

    /**
     * @brief Component that holds a shared pointer to a Renderable resource.
     *
     * @details
     * This component links a GameObject to a visual representation (Renderable).
     * When attached, it automatically extracts the AABB from the renderable's mesh
     * and populates a ModelAabbComponent on the same GameObject.
     */
    class RenderableComponent  {

        /**
         * @brief Shared pointer to the renderable resource.
         */
        std::shared_ptr<helios::rendering::Renderable> renderable_;

        /**
         * @brief The initial AABB of the model associated with the renderable.
         */
       helios::math::aabbf aabb_{};

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        /**
         * @brief Constructs a RenderableComponent.
         *
         * @param renderable Shared pointer to the Renderable. Must not be nullptr.
         */
        explicit RenderableComponent(std::shared_ptr<helios::rendering::Renderable> renderable) :
            renderable_(std::move(renderable)) {

            assert(renderable_ != nullptr && "renderable must not be nullptr");
            aabb_ = renderable_->localAABB();

        }

        /**
         * @brief Copy constructor.
         *
         * @details Copies the shared pointer to the Renderable. Both components
         * will reference the same Renderable resource (shared ownership).
         *
         * @param other The component to copy from.
         */
        RenderableComponent(const RenderableComponent& other)  :
        renderable_(other.renderable_), aabb_(other.aabb_) {}

        /**
         * @brief Copy assignment operator.
         */
        RenderableComponent& operator=(const RenderableComponent&) = default;

        /**
         * @brief Move constructor.
         */
        RenderableComponent(RenderableComponent&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         */
        RenderableComponent& operator=(RenderableComponent&&) noexcept = default;



        /**
         * @brief Retrieves a shared pointer to the stored Renderable.
         *
         * @return Shared pointer to the Renderable.
         */
        [[nodiscard]] std::shared_ptr<helios::rendering::Renderable> shareRenderable() const noexcept {
            return renderable_;
        }

        /**
         * @brief Retrieves a const ref to the stored Renderable.
         *
         * @return Const ref to the Renderable (never null after construction).
         */
        [[nodiscard]] const helios::rendering::Renderable& renderable() const noexcept {
            return *renderable_;
        }



    };


}