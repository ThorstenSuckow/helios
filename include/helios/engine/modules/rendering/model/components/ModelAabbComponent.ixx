/**
 * @file ModelAabbComponent.ixx
 * @brief Component for storing the canonical AABB of a model.
 */
module;

export module helios.engine.modules.rendering.model.components.ModelAabbComponent;

import helios.math.types;
import helios.engine.ecs.CloneableComponent;
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
    class ModelAabbComponent : public helios::engine::ecs::CloneableComponent<ModelAabbComponent> {

        /**
         * @brief The stored AABB.
         */
        helios::math::aabbf aabb_{};


    public:

        /**
         * @brief Default constructor.
         */
        ModelAabbComponent() = default;

        explicit ModelAabbComponent(const ModelAabbComponent& other) :
        aabb_(other.aabb_) {}

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