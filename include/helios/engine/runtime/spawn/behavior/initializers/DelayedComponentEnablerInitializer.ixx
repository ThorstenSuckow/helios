/**
 * @file DelayedComponentEnablerInitializer.ixx
 * @brief Initializer that schedules delayed activation of specified components.
 */
module;

#include <cassert>
#include <typeindex>
#include <vector>

export module helios.engine.runtime.spawn.behavior.initializers.DelayedComponentEnablerInitializer;


import helios.engine.runtime.spawn.behavior.SpawnInitializer;
import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.ecs.GameObject;
import helios.engine.ecs.Component;
import helios.engine.mechanics.lifecycle.components.DelayedComponentEnabler;
import helios.engine.core.data.ComponentTypeId;

export namespace helios::engine::runtime::spawn::behavior::initializers {

    /**
     * @brief Initializer that schedules delayed activation of arbitrary components.
     *
     * @details DelayedComponentEnablerInitializer defers the activation of specified
     * components until a calculated delay has elapsed. This enables gameplay patterns
     * such as:
     * - **Collision immunity:** Newly spawned entities ignore collisions briefly
     * - **Visibility fade-in:** Rendering components activate after spawn animation
     * - **AI activation:** Behavior components enable after entity is fully placed
     * - **Staggered waves:** Sequential spawns activate progressively
     *
     * The delay for each entity is calculated as:
     * `delay = (batchPosition + 1) * baseDelay`
     *
     * An optional cycle length resets the batch position via modulo, useful for
     * repeating spawn patterns where delay should not grow indefinitely.
     *
     * @tparam ComponentTypes The component types whose activation should be deferred.
     *
     * @note The entity must have a DelayedComponentEnabler component attached.
     *       The specified ComponentTypes must exist on the entity.
     *
     * @see SpawnInitializer
     * @see DelayedComponentEnabler
     */
    template<typename... ComponentTypes>
    class DelayedComponentEnablerInitializer final : public SpawnInitializer {

        /**
         * @brief Base delay in seconds between activations.
         */
        const float delay_;

        /**
         * @brief Cycle length for repeating delay patterns.
         *
         * If non-zero, position indices wrap around after this many spawns.
         * If zero, positions increase indefinitely.
         */
        const size_t cycleLength_;

        /**
         * @brief Type IDs of components whose activation should be deferred.
         */
        std::vector<helios::engine::core::data::ComponentTypeId> deferredComponents_;

    public:

        /**
         * @brief Constructs the initializer with delay and optional cycle length.
         *
         * @param delay Base delay in seconds between each activation step.
         * @param cycleLength Number of positions before the delay pattern repeats.
         *                    Zero means no cycling (default).
         */
        DelayedComponentEnablerInitializer(const float delay, const size_t cycleLength = 0) :
            delay_(delay),
            cycleLength_(cycleLength),
            deferredComponents_{helios::engine::core::data::ComponentTypeId::id<ComponentTypes>() ...}
        {}

        /**
         * @brief Schedules delayed activation of the configured component types.
         *
         * @details Searches the entity's components for matches against the template
         * ComponentTypes. Each matching component is registered with the entity's
         * DelayedComponentEnabler for deferred activation. The delay duration depends
         * on the spawn batch position and the configured base delay.
         *
         * @param gameObject The entity to initialize.
         * @param cursor Provides batch position for delay calculation.
         * @param spawnContext Context data (unused).
         */
        void initialize(
            const helios::engine::ecs::GameObject& gameObject,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            auto components = gameObject.components();
            bool deferFound = false;

            auto position = cycleLength_ != 0 ?  cursor.position % cycleLength_ : cursor.position;

            for (auto& component : components) {

                const auto typeId = component->typeId();

                // Find the component that should be deferred.
                const bool deferThisComponent = std::find(
                    deferredComponents_.begin(), deferredComponents_.end(), typeId
                ) != deferredComponents_.end();

                if (deferThisComponent) {
                    deferFound = true;
                    auto* dec = gameObject.get<helios::engine::mechanics::lifecycle::components::DelayedComponentEnabler>();
                    assert(dec && "Missing DelayedComponentEnabler");

                    dec->defer(typeId, (position + 1) * delay_);
                }
            }

            assert(deferFound && "Unexpected missing deferrable component");
        }

    };

}


