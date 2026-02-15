/**
 * @file MenuBuilder.ixx
 * @brief Builder for creating menu and menu item configurations.
 */
module;

export module helios.engine.builder.gameObject.builders.MenuBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.MenuConfig;
import helios.engine.builder.gameObject.builders.configs.MenuItemConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for creating menu and menu item configurations.
     *
     * Provides factory methods to create MenuConfig and MenuItemConfig
     * instances for fluent menu construction.
     */
    class MenuBuilder {

        /**
         * @brief The GameObject being configured.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a MenuBuilder for the given GameObject.
         *
         * @param gameObject The GameObject to configure.
         */
        explicit MenuBuilder(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Creates a menu configuration for this GameObject.
         *
         * @return A MenuConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::MenuConfig menu() {
            return helios::engine::builder::gameObject::builders::configs::MenuConfig{gameObject_};
        }

        /**
         * @brief Creates a menu item configuration attached to a parent menu.
         *
         * @param parentMenu The parent menu GameObject (must have MenuComponent).
         *
         * @return A MenuItemConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::MenuItemConfig menuItem(
         helios::engine::ecs::GameObject& parentMenu
        ) {
            return helios::engine::builder::gameObject::builders::configs::MenuItemConfig{gameObject_, parentMenu};
        }

    };

}
