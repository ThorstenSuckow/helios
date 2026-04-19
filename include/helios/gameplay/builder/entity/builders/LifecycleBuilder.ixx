/**
 * @file LifecycleBuilder.ixx
 * @brief Builder for configuring lifecycle-related components on Entities.
 */
module;

export module helios.gameplay.builder.entity.builders.LifecycleBuilder;

import helios.gameplay.builder.entity.builders.configs.LifecycleConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for lifecycle management and component activation timing.
     *
     * @details LifecycleBuilder provides configuration options for controlling
     * when and how components become active on a Entity. This is essential
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
    template<typename Entity>
    class LifecycleBuilder {

        Entity entity_;

    public:

        /**
         * @brief Constructs a LifecycleBuilder for the given Entity.
         *
         * @param entity Non-owning pointer to the target entity.
         */
        explicit LifecycleBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a configuration object for lifecycle settings.
         *
         * @details The returned LifecycleConfig allows configuring delayed
         * component activation and other lifecycle behaviors.
         *
         * @return LifecycleConfig instance targeting the same Entity.
         */
        helios::gameplay::builder::entity::builders::configs::LifecycleConfig<Entity> lifecycle() {
            return helios::gameplay::builder::entity::builders::configs::LifecycleConfig<Entity>{entity_};
        }


    };

}
