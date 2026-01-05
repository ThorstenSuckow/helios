/**
 * @file LevelBoundsBehaviorComponent.ixx
 * @brief Component defining behavior when an entity interacts with level boundaries.
 */
module;

#include <algorithm>
#include <cassert>

export module helios.engine.game.physics.components.LevelBoundsBehaviorComponent;

import helios.engine.game.Component;

export namespace helios::engine::game::physics::components {

    /**
     * @brief Component that defines how an entity reacts to level boundaries.
     *
     * @details
     * This component stores properties related to boundary interactions, such as
     * restitution (bounciness) when colliding with level walls, used by physics
     * or movement systems to resolve out-of-bounds conditions.
     */
    class LevelBoundsBehaviorComponent : public helios::engine::game::Component {

    private:
        /**
         * @brief Coefficient of restitution (bounciness).
         *
         * @details
         * A value of 1.0 means a perfectly elastic collision (no energy lost).
         * A value of 0.0 means a perfectly inelastic collision (no bounce).
         * Default is 0.5.
         */
        float restitution_ = 0.5f;

    public:

        /**
         * @brief Default constructor with default restitution (0.5).
         */
        LevelBoundsBehaviorComponent() = default;

        /**
         * @brief Constructs a LevelBoundsBehaviorComponent with a specified restitution.
         *
         * @param restitution The coefficient of restitution (0.0 to 1.0).
         */
        explicit LevelBoundsBehaviorComponent(float restitution) : Component(), restitution_(restitution) {}

        /**
         * @brief Retrieves the restitution coefficient.
         *
         * @return The restitution value.
         */
        [[nodiscard]] float restitution() const noexcept {
            return restitution_;
        }

        /**
         * @brief Sets the coefficient of restitution (bounciness).
         *
         * @param restitution The new restitution value to set. Must be within the
         * range [0.0, 1.0]. Values outside this range will be clamped.
         */
        void setRestitution(const float restitution) noexcept {
            assert(restitution >= 0.0f && restitution <= 1.0f && "Unexpected value for restitution");
            restitution_ = std::clamp(restitution, 0.0f, 1.0f);
        }


    };


}