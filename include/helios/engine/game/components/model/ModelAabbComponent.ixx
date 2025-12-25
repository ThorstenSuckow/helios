module;

export module helios.engine.game.components.model.ModelAabbComponent;

import helios.math.types;
import helios.engine.game.Component;
import helios.core.Transform;

export namespace helios::engine::game::components::model {

    /**
     * @brief Component that stores the canonical Axis-Aligned Bounding Box (AABB) of a model.
     *
     * @details
     * This component is used to associate spatial bounds with an entity, typically derived
     * from a 3D model or mesh. It is essential for culling, collision detection, and
     * other spatial queries.
     */
    class ModelAabbComponent : public helios::engine::game::Component {

        /**
         * @brief The stored AABB.
         */
        helios::math::aabbf aabb_{};


    public:

        /**
         * @brief Default constructor.
         */
        ModelAabbComponent() = default;

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