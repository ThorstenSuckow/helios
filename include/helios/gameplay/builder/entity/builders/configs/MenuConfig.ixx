/**
 * @file MenuConfig.ixx
 * @brief Fluent configuration for menu Entities.
 */
module;

#include <cassert>

export module helios.gameplay.builder.entity.builders.configs.MenuConfig;


import helios.ui.widgets.types.MenuId;

import helios.ui.widgets.components.MenuComponent;


export namespace helios::gameplay::builder::entity::builders::configs {

    /**
     * @brief Fluent configuration for menu Entities.
     *
     * Provides a builder-style interface for configuring menu properties
     * on a Entity.
     */
    template<typename Entity>
    class MenuConfig {

        /**
         * @brief The Entity being configured.
         */
        Entity entity_;

        using Handle_type = typename Entity::Handle_type;

    public:

        /**
         * @brief Constructs a MenuConfig for the given Entity.
         *
         * @param entity The Entity to configure.
         */
        explicit MenuConfig(Entity entity) : entity_(entity) {

        }

        /**
         * @brief Sets the menu identifier.
         *
         * @param menuId The menu ID to assign.
         *
         * @return Reference to this config for method chaining.
         */
        MenuConfig& menuId(const helios::ui::widgets::types::MenuId menuId) {

            entity_.template getOrAdd<helios::ui::widgets::components::MenuComponent<Handle_type>>()
                        .setMenuId(menuId);

            return *this;
        }

    };

}
