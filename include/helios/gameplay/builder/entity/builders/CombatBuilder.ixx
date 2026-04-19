/**
 * @file CombatBuilder.ixx
 * @brief Builder for weapon and combat component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.CombatBuilder;

import helios.gameplay.builder.entity.builders.configs.WeaponConfig;
import helios.gameplay.builder.entity.builders.configs.CombatConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring weapon and projectile components.
     *
     * Provides factory methods for setting up weapon systems
     * including fire rate and projectile spawning.
     */
    template<typename Entity>
    class CombatBuilder {

        Entity entity_;

    public:

        /**
         * @brief Constructs a CombatBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit CombatBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for weapon setup.
         *
         * @return WeaponConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::WeaponConfig<Entity> weapon() const {
            return helios::gameplay::builder::entity::builders::configs::WeaponConfig<Entity>{entity_};
        }

        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::CombatConfig<Entity> combat() const {
            return helios::gameplay::builder::entity::builders::configs::CombatConfig<Entity>{entity_};
        }

    };

}
