/**
 * @file MenuBuilder.ixx
 * @brief Builder for creating menu and menu item configurations.
 */
module;

export module helios.gameplay.builder.gameObject.builders.MenuBuilder;

import helios.gameplay.builder.gameObject.builders.configs.MenuConfig;
import helios.gameplay.builder.gameObject.builders.configs.MenuItemConfig;

export namespace helios::gameplay::builder::gameObject::builders {

    /**
     * @brief Builder for creating menu and menu item configurations.
     *
     * Provides factory methods to create MenuConfig and MenuItemConfig
     * instances for fluent menu construction.
     */
    template<typename Entity>
    class MenuBuilder {

        /**
         * @brief The GameObject being configured.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a MenuBuilder for the given GameObject.
         *
         * @param gameObject The GameObject to configure.
         */
        explicit MenuBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Creates a menu configuration for this GameObject.
         *
         * @return A MenuConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::MenuConfig<Entity> menu() {
            return helios::gameplay::builder::gameObject::builders::configs::MenuConfig<Entity>{gameObject_};
        }

        /**
         * @brief Creates a menu item configuration attached to a parent menu.
         *
         * @param parentMenu The parent menu GameObject (must have MenuComponent).
         *
         * @return A MenuItemConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::MenuItemConfig<Entity> menuItem(
         Entity& parentMenu
        ) {
            return helios::gameplay::builder::gameObject::builders::configs::MenuItemConfig<Entity>{gameObject_, parentMenu};
        }

    };

}
