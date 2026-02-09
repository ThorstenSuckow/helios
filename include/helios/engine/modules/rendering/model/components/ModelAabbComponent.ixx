/**
 * @file ModelAabbComponent.ixx
 * @brief Component for storing the canonical AABB of a model.
 */
module;

export module helios.engine.modules.rendering.model.components.ModelAabbComponent;

import helios.math.types;

import helios.core.spatial.Transform;

export namespace helios::engine::modules::rendering::model::components {

    /**
     * @brief Component that stores the canonical Axis-Aligned Bounding Box (AABB) of a model.
     *
     * @details
     * This component is used to associate spatial bounds with an entity, typically derived
     * from a 3D model or mesh. It is essential for culling, collision detection, and
     * other spatial queries.
     */
    class ModelAabbComponent  {

        /**
         * @brief The stored AABB.
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
         * @brief Default constructor.
         */
        ModelAabbComponent() = default;

        ModelAabbComponent(const ModelAabbComponent& other) :
        aabb_(other.aabb_) {}

        ModelAabbComponent& operator=(const ModelAabbComponent&) = default;
        ModelAabbComponent(ModelAabbComponent&&) noexcept = default;
        ModelAabbComponent& operator=(ModelAabbComponent&&) noexcept = default;

        /**
         * @brief Sets the AABB for this component.
         *
         * @param aabb The new AABB to store.
         */
        void setAabb(const helios::math::aabbf& aabb) {
            aabb_ = aabb;
        }


        /**
         * @brief Retrieves the stored AABB.
         *
         * @return Constant reference to the AABB.
         */
        [[nodiscard]] const helios::math::aabbf& aabb() const noexcept {
            return aabb_;
        }
    };

}