/**
 * @file MenuBuilder.ixx
 * @brief Builder for creating menu and menu item configurations.
 */
module;

export module helios.gameplay.builder.entity.builders.MenuBuilder;

import helios.gameplay.builder.entity.builders.configs.MenuConfig;
import helios.gameplay.builder.entity.builders.configs.MenuItemConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for creating menu and menu item configurations.
     *
     * Provides factory methods to create MenuConfig and MenuItemConfig
     * instances for fluent menu construction.
     */
    template<typename Entity>
    class MenuBuilder {

        /**
         * @brief The Entity being configured.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a MenuBuilder for the given Entity.
         *
         * @param entity The Entity to configure.
         */
        explicit MenuBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Creates a menu configuration for this Entity.
         *
         * @return A MenuConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::MenuConfig<Entity> menu() {
            return helios::gameplay::builder::entity::builders::configs::MenuConfig<Entity>{entity_};
        }

        /**
         * @brief Creates a menu item configuration attached to a parent menu.
         *
         * @param parentMenu The parent menu Entity (must have MenuComponent).
         *
         * @return A MenuItemConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::MenuItemConfig<Entity> menuItem(
         Entity& parentMenu
        ) {
            return helios::gameplay::builder::entity::builders::configs::MenuItemConfig<Entity>{entity_, parentMenu};
        }

    };

}
