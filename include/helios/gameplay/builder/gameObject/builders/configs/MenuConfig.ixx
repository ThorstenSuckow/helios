/**
 * @file MenuConfig.ixx
 * @brief Fluent configuration for menu GameObjects.
 */
module;

#include <cassert>

export module helios.gameplay.builder.gameObject.builders.configs.MenuConfig;


import helios.ui.widgets.types.MenuId;

import helios.ui.widgets.components.MenuComponent;


export namespace helios::gameplay::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for menu GameObjects.
     *
     * Provides a builder-style interface for configuring menu properties
     * on a GameObject.
     */
    template<typename Entity>
    class MenuConfig {

        /**
         * @brief The GameObject being configured.
         */
        Entity gameObject_;

        using Handle_type = typename Entity::Handle_type;

    public:

        /**
         * @brief Constructs a MenuConfig for the given GameObject.
         *
         * @param gameObject The GameObject to configure.
         */
        explicit MenuConfig(Entity gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Sets the menu identifier.
         *
         * @param menuId The menu ID to assign.
         *
         * @return Reference to this config for method chaining.
         */
        MenuConfig& menuId(const helios::ui::widgets::types::MenuId menuId) {

            gameObject_.template getOrAdd<helios::ui::widgets::components::MenuComponent<Handle_type>>()
                        .setMenuId(menuId);

            return *this;
        }

    };

}
