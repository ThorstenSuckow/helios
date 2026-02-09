/**
 * @file EmittedByComponent.ixx
 * @brief Component tracking the source entity that emitted a spawned object.
 */
module;

export module helios.engine.mechanics.spawn.components.EmittedByComponent;


import helios.engine.core.data.SpawnProfileId;
import helios.core.types;
import helios.util;

import helios.engine.ecs.EntityHandle;

export namespace helios::engine::mechanics::spawn::components {


    /**
     * @brief Component that tracks which entity emitted/spawned this object.
     *
     * Used to establish a relationship between a spawned entity (e.g., a projectile)
     * and its source entity (e.g., the player or enemy that fired it). This enables
     * game logic to attribute actions like damage or scoring to the correct source.
     */
    class EmittedByComponent {

        /**
         * @brief Handle of the entity that emitted this object.
         */
        helios::engine::ecs::EntityHandle source_{};

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
        EmittedByComponent() = default;

        /**
         * @brief Copy constructor (creates empty state).
         *
         * @param other The component to copy from (state is not copied).
         */
        EmittedByComponent(const EmittedByComponent& other) {}

        EmittedByComponent& operator=(const EmittedByComponent&) = default;
        EmittedByComponent(EmittedByComponent&&) noexcept = default;
        EmittedByComponent& operator=(EmittedByComponent&&) noexcept = default;

        /**
         * @brief Sets the source entity that emitted this object.
         *
         * @param source Handle of the emitting entity.
         */
        void setSource(const helios::engine::ecs::EntityHandle source) noexcept {
            source_ = source;
        }

        /**
         * @brief Returns the handle of the source entity.
         *
         * @return Handle of the entity that emitted this object.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle source() const noexcept {
            return source_;
        }

        /**
         * @brief Resets the component to its initial state.
         */
        void reset() {
            source_ = helios::engine::ecs::EntityHandle{};
        }

        /**
         * @brief Called when this entity is acquired from a pool.
         *
         * @details Resets the source handle to prevent stale references.
         */
        void onAcquire() noexcept {
            reset();
        }

        /**
         * @brief Called when this entity is released back to a pool.
         *
         * @details Resets the source handle to prevent stale references.
         */
        void onRelease() noexcept {
            reset();
        }

    };

}