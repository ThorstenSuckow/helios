/**
 * @file EmittedByComponent.ixx
 * @brief Component tracking the source entity that emitted a spawned object.
 */
module;

export module helios.engine.mechanics.spawn.components.EmittedByComponent;

import helios.engine.ecs.CloneableComponent;
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
    class EmittedByComponent : public helios::engine::ecs::CloneableComponent<EmittedByComponent> {

        /**
         * @brief Handle of the entity that emitted this object.
         */
        helios::engine::ecs::EntityHandle source_{};

    public:

        /**
         * @brief Default constructor.
         */
        EmittedByComponent() = default;

        /**
         * @brief Copy constructor (creates empty state).
         *
         * @param other The component to copy from (state is not copied).
         */
        explicit EmittedByComponent(const EmittedByComponent& other) {}

        /**
         * @brief Sets the source entity that emitted this object.
         *
         * @param source Handle of the emitting entity.
         */
        void setSource(const helios::engine::ecs::EntityHandle& source) noexcept {
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
         * @copydoc CloneableComponent::onAcquire()
         */
        void onAcquire() noexcept override {
            reset();
        }

        /**
         * @copydoc CloneableComponent::onRelease()
         */
        void onRelease() noexcept override {
            reset();
        }

    };

}