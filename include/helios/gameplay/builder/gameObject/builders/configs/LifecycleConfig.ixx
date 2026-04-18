/**
 * @file LifecycleConfig.ixx
 * @brief Configuration class for lifecycle and delayed component activation.
 */
module;

#include <cassert>
#include <memory>

export module helios.gameplay.builder.gameObject.builders.configs.LifecycleConfig;


import helios.gameplay.lifecycle.components.DelayedComponentEnabler;


export namespace helios::gameplay::builder::gameObject::builders::configs {

    /**
     * @brief Configuration for lifecycle management on a GameObject.
     *
     * @details LifecycleConfig provides a fluent interface for setting up
     * delayed component activation. When combined with spawn initializers,
     * this enables powerful patterns like collision immunity windows and
     * staggered wave activation.
     *
     * ## Usage
     *
     * ```cpp
     * LifecycleConfig{gameObject}
     *     .useDelayedComponentEnabler();
     * ```
     *
     * ## Integration with Spawn System
     *
     * Use with `DelayedComponentEnablerInitializer` to defer specific
     * components during spawn:
     *
     * ```cpp
     * // In prototype setup
     * prototype.withLifecycle([](LifecycleBuilder& builder) {
     *     builder.lifecycle().useDelayedComponentEnabler();
     * });
     *
     * // In spawn profile
     * spawnProfile.spawnInitializer = std::make_unique<DelayedComponentEnablerInitializer<
     *     CollisionComponent>>(0.5f, 25);
     * ```
     *
     * @see DelayedComponentEnabler
     * @see DelayedComponentEnablerSystem
     * @see DelayedComponentEnablerInitializer
     */
    template<typename Entity>
    class LifecycleConfig {

        Entity& gameObject_;

        using Handle_type = typename Entity::Handle_type;

    public:

        /**
         * @brief Constructs a LifecycleConfig for the given GameObject.
         *
         * @param gameObject Non-owning pointer to the target entity.
         */
        explicit LifecycleConfig(Entity& gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Enables delayed component activation for this entity.
         *
         * @details Adds a DelayedComponentEnabler component if not already present.
         * This component maintains a queue of components scheduled for delayed
         * activation, which is processed by the DelayedComponentEnablerSystem.
         *
         * @return Reference to this config for method chaining.
         */
        LifecycleConfig& useDelayedComponentEnabler() {

            gameObject_.template getOrAdd<helios::gameplay::lifecycle::components::DelayedComponentEnabler<Handle_type>>();

            return *this;
        }



    };

}
