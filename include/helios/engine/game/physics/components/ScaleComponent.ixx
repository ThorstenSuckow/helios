/**
 * @file ScaleComponent.ixx
 * @brief Component for managing the dimensions of an entity.
 */
module;

export module helios.engine.game.physics.components.ScaleComponent;

import helios.math.types;
import helios.engine.game.Component;
import helios.core.Transform;
import helios.core.units;


export namespace helios::engine::game::physics::components {

    /**
     * @brief Component that defines the physical dimensions (scale) of an entity.
     *
     * @details
     * This component stores the width, height, and depth of an entity, along with the
     * unit of measurement. It tracks changes via a dirty flag, allowing other systems
     * to react to scale updates.
     */
    class ScaleComponent : public helios::engine::game::Component {

        /**
         * @brief Width of the entity.
         */
        float width_;

        /**
         * @brief Height of the entity.
         */
        float height_;

        /**
         * @brief Depth of the entity.
         */
        float depth_;

        /**
         * @brief Unit of measurement for the dimensions.
         */
        helios::core::units::Unit unit_;

        /**
         * @brief Flag indicating if the scale has changed.
         */
        bool isDirty_ = true;

    public:

        /**
         * @brief Constructs a ScaleComponent with specified dimensions and unit.
         *
         * @param width Width of the entity.
         * @param height Height of the entity.
         * @param depth Depth of the entity.
         * @param unit Unit of measurement (default: Meter).
         */
        explicit ScaleComponent(const float width, const float height, const float depth, helios::core::units::Unit unit) :
        width_(width), height_(height), depth_(depth), unit_(unit) {}

        /**
         * @brief Called when the component is attached to a GameObject.
         *
         * @param gameObject Pointer to the parent GameObject.
         */
        void onAttach(GameObject* gameObject) noexcept override {
            Component::onAttach(gameObject);
        }

        /**
         * @brief Clears the dirty flag.
         */
        void clearDirty() noexcept {
            isDirty_ = false;
        }

        /**
         * @brief Checks if the scale has changed.
         *
         * @return True if dirty, false otherwise.
         */
        [[nodiscard]] bool isDirty() const noexcept {
            return isDirty_;
        }


        /**
         * @brief Retrieves the unit of measurement.
         *
         * @return The unit enum value.
         */
        [[nodiscard]] helios::core::units::Unit unit() const noexcept {
            return unit_;
        }

        /**
         * @brief Retrieves the scaling vector (width, height, depth).
         *
         * @return A vec3f containing the dimensions.
         */
        [[nodiscard]] helios::math::vec3f scaling() const noexcept {
            return {width_, height_, depth_};
        }

        /**
         * @brief Sets the dimensions and unit of the entity.
         *
         * @param width New width.
         * @param height New height.
         * @param depth New depth.
         * @param unit New unit of measurement (default: Meter).
         */
        void setScale(const float width, const float height, const float depth,
                      const helios::core::units::Unit unit = helios::core::units::Unit::Meter
        ) noexcept {

            width_ = width;
            height_ = height;
            depth_ = depth;
            unit_ = unit;
            isDirty_ = true;

        };


    };

}
