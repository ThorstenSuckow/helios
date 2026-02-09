/**
 * @file DelayedComponentEnabler.ixx
 * @brief Component for scheduling delayed activation of other components.
 */
module;

#include <cassert>
#include <span>
#include <typeindex>
#include <memory>
#include <vector>

export module helios.engine.mechanics.lifecycle.components.DelayedComponentEnabler;



import helios.engine.ecs.GameObject;
import helios.engine.core.data.SpawnProfileId;
import helios.engine.core.data.ComponentTypeId;
import helios.core.types;

export namespace helios::engine::mechanics::lifecycle::components {

    /**
     * @brief Component that manages delayed activation of other components.
     *
     * @details DelayedComponentEnabler maintains a list of components that should
     * be activated after a specified delay. This is useful for gameplay patterns
     * where newly spawned entities need temporary immunity or staggered activation:
     * - **Collision immunity:** Projectiles ignore collisions briefly after spawn
     * - **AI warmup:** Behavior components activate after positioning completes
     * - **Visual fade-in:** Rendering components enable after spawn effects
     * - **Staggered waves:** Sequential entities activate progressively
     *
     * The component works in conjunction with DelayedComponentEnablerSystem, which
     * decrements timers each frame and enables components when their delay expires.
     *
     * @note Components are disabled immediately when deferred and re-enabled
     *       automatically by the system when the delay reaches zero.
     *
     * @see DelayedComponentEnablerSystem
     * @see DelayedComponentEnablerInitializer
     */
    class DelayedComponentEnabler {

        /**
         * @brief Internal structure tracking a deferred component.
         */
        struct DeferredComponent {
            /**
             * @brief Remaining time in seconds until activation.
             */
            float delta;

            /**
             * @brief Type identifier of the deferred component.
             */
            helios::engine::core::data::ComponentTypeId componentTypeId;
        };

        /**
         * @brief List of components pending delayed activation.
         */
        std::vector<DeferredComponent> deferredComponents_;

    public:

        /**
         * @brief Default constructor.
         */
        DelayedComponentEnabler() = default;

        /**
         * @brief Copy constructor for cloning during spawn.
         *
         * @param other The source component to copy from.
         */
        DelayedComponentEnabler(const DelayedComponentEnabler& other)
            : deferredComponents_(other.deferredComponents_) {}

        DelayedComponentEnabler& operator=(const DelayedComponentEnabler&) = default;
        DelayedComponentEnabler(DelayedComponentEnabler&&) noexcept = default;
        DelayedComponentEnabler& operator=(DelayedComponentEnabler&&) noexcept = default;

        /**
         * @brief Returns a view of all currently deferred components.
         *
         * @return Span of DeferredComponent entries with remaining delays.
         */
        std::span<DeferredComponent> deferredComponents() noexcept {
            return deferredComponents_;
        }

        /**
         * @brief Removes activated components from the deferred list.
         *
         * @details Called by the system after enabling components to clean up
         * entries that have completed their delay period.
         *
         * @param removeList Component type IDs to remove from tracking.
         */
        void sync(std::span<helios::engine::core::data::ComponentTypeId> removeList) {
            std::erase_if(deferredComponents_, [&](const DeferredComponent& dc) {
                return std::ranges::find(removeList, dc.componentTypeId) != removeList.end();
            });
        }

        /**
         * @brief Schedules a component for delayed activation.
         *
         * @details The specified component is immediately disabled and added to
         * the deferred list. If the component is already deferred, its delay is
         * updated to the new value.
         *
         * @param componentTypeId Type identifier of the component to defer.
         * @param delta Delay in seconds before activation.
         *
         * @note Asserts if delta <= 0 or if the component does not exist on the entity.
         */
        void defer(
            helios::engine::ecs::GameObject gameObject,
            helios::engine::core::data::ComponentTypeId componentTypeId, const float delta) {
            assert(delta > 0 && "delta must be greater than 0");

            const bool hasCmp  = gameObject.has(componentTypeId);
            assert(hasCmp && "ComponentTypeId not part of GameObject");

            const auto it = std::ranges::find_if(deferredComponents_,
                [componentTypeId](const auto& item) {
                return item.componentTypeId == componentTypeId;
            });

            gameObject.disableComponent(componentTypeId);

            if (it == deferredComponents_.end()) {
                deferredComponents_.push_back({delta, componentTypeId});
            } else {
                it->delta = delta;
            }
        }


    };

}