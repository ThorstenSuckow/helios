/**
 * @file MenuConfig.ixx
 * @brief Fluent configuration for menu GameObjects.
 */
module;

#include <cassert>

export module helios.engine.builder.gameObject.builders.configs.MenuConfig;

import helios.engine.ecs.GameObject;

import helios.engine.core.data;

import helios.engine.modules.ui.widgets.components.MenuComponent;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for menu GameObjects.
     *
     * Provides a builder-style interface for configuring menu properties
     * on a GameObject.
     */
    class MenuConfig {

        /**
         * @brief The GameObject being configured.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a MenuConfig for the given GameObject.
         *
         * @param gameObject The GameObject to configure.
         */
        explicit MenuConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Sets the menu identifier.
         *
         * @param menuId The menu ID to assign.
         *
         * @return Reference to this config for method chaining.
         */
        MenuConfig& menuId(const helios::engine::core::data::MenuId menuId) {

            gameObject_.getOrAdd<helios::engine::modules::ui::widgets::components::MenuComponent>()
                        .setMenuId(menuId);

            return *this;
        }

    };

}
