/**
 * @file LifecycleBuilder.ixx
 * @brief Builder for configuring lifecycle-related components on GameObjects.
 */
module;

export module helios.engine.builder.gameObject.builders.LifecycleBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.LifecycleConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for lifecycle management and component activation timing.
     *
     * @details LifecycleBuilder provides configuration options for controlling
     * when and how components become active on a GameObject. This is essential
     * for gameplay patterns requiring delayed or staged component activation:
     *
     * - **Spawn immunity:** Defer collision detection until entity is positioned
     * - **Staggered activation:** Enable behaviors progressively across wave spawns
     * - **Visual polish:** Delay rendering until spawn effects complete
     *
     * ## Usage
     *
     * ```cpp
     * prototype.withLifecycle([](LifecycleBuilder& builder) {
     *     builder.lifecycle()
     *            .useDelayedComponentEnabler();
     * });
     * ```
     *
     * @see LifecycleConfig
     * @see DelayedComponentEnabler
     * @see DelayedComponentEnablerInitializer
     */
    class LifecycleBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a LifecycleBuilder for the given GameObject.
         *
         * @param gameObject Non-owning pointer to the target entity.
         */
        explicit LifecycleBuilder(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a configuration object for lifecycle settings.
         *
         * @details The returned LifecycleConfig allows configuring delayed
         * component activation and other lifecycle behaviors.
         *
         * @return LifecycleConfig instance targeting the same GameObject.
         */
        helios::engine::builder::gameObject::builders::configs::LifecycleConfig lifecycle() {
            return helios::engine::builder::gameObject::builders::configs::LifecycleConfig{gameObject_};
        }


    };

}
