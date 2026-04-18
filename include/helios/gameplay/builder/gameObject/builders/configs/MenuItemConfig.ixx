/**
 * @file MenuItemConfig.ixx
 * @brief Fluent configuration for menu item GameObjects.
 */
module;

#include <cassert>
#include <optional>

export module helios.gameplay.builder.gameObject.builders.configs.MenuItemConfig;


import helios.ui.widgets.types;
import helios.ui.widgets.components.MenuComponent;
import helios.ui.widgets.components.UiStyleComponent;
import helios.ui.widgets.components.UiStateComponent;
import helios.ui.widgets.components.UiActionComponent;

import helios.math;


export namespace helios::gameplay::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for menu item GameObjects.
     *
     * Provides a builder-style interface for configuring menu items including
     * styling, selection state, and action binding. Automatically attaches
     * the item to the parent menu.
     */
    template<typename Entity>
    class MenuItemConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief The GameObject being configured.
         */
        Entity gameObject_;

        /**
         * @brief Pointer to the parent menu GameObject.
         */
        Entity* parentMenu_ = nullptr;

        /**
         * @brief Optional index position in the menu.
         */
        std::optional<size_t> index_ = std::nullopt;

        /**
         * @brief Whether this item is selected.
         */
        bool isSelected_ = false;

        /**
         * @brief Attaches this item to the parent menu.
         */
        void attach() {
            assert(parentMenu_ != nullptr);

            auto* mc = parentMenu_->get<helios::ui::widgets::components::MenuComponent<Handle_type>>();

            assert(mc != nullptr && "Unexpected nullptr for MenuComponent");

            mc->addMenuItem(gameObject_.handle());
        }
    public:

        /**
         * @brief Constructs a MenuItemConfig for the given GameObject.
         *
         * Automatically adds UiStyleComponent and UiStateComponent,
         * and attaches the item to the parent menu.
         *
         * @param gameObject The GameObject to configure.
         * @param parentMenu The parent menu GameObject (must have MenuComponent).
         */
        explicit MenuItemConfig(
            Entity gameObject,
            Entity& parentMenu
        ) : gameObject_(gameObject), parentMenu_(&parentMenu) {


            gameObject_.template getOrAdd<helios::ui::widgets::components::UiStyleComponent<Handle_type>>();
            gameObject_.template getOrAdd<helios::ui::widgets::components::UiStateComponent<Handle_type>>();


            assert(parentMenu_->template has<helios::ui::widgets::components::MenuComponent<Handle_type>>() &&
                "parent menu must have MenuComponent");

            attach();
        }

        /**
         * @brief Sets the selected state.
         *
         * @param isSelected True if this item should be selected.
         *
         * @return Reference to this config for method chaining.
         */
        MenuItemConfig& selected(const bool isSelected) {

            if (!isSelected_) {
                return *this;

            }
            if (index_) {
                parentMenu_->get<helios::ui::widgets::components::MenuComponent<Handle_type>>()
                           ->setSelectedIndex(index_.value());
            } else {
                isSelected_ = isSelected;
            }

            return *this;
        }

        /**
         * @brief Sets the normal state color.
         *
         * @param color The RGBA color.
         *
         * @return Reference to this config for method chaining.
         */
        MenuItemConfig& normalColor(const helios::math::vec4f color) {
            gameObject_.get<helios::ui::widgets::components::UiStyleComponent<Handle_type>>()
                       ->setNormalColor(color);

            return *this;
        }

        /**
         * @brief Sets the normal state scale.
         *
         * @param scale The scale factor.
         *
         * @return Reference to this config for method chaining.
         */
        MenuItemConfig& normalScale(const float scale) {
            gameObject_.get<helios::ui::widgets::components::UiStyleComponent<Handle_type>>()
                       ->setNormalScale(scale);

            return *this;
        }

        /**
         * @brief Sets the selected state color.
         *
         * @param color The RGBA color.
         *
         * @return Reference to this config for method chaining.
         */
        MenuItemConfig& selectedColor(helios::math::vec4f color) {
            gameObject_.get<helios::ui::widgets::components::UiStyleComponent<Handle_type>>()
                       ->setSelectedColor(color);

            return *this;
        }

        /**
         * @brief Sets the selected state scale.
         *
         * @param scale The scale factor.
         *
         * @return Reference to this config for method chaining.
         */
        MenuItemConfig& selectedScale(const float scale) {
            gameObject_.get<helios::ui::widgets::components::UiStyleComponent<Handle_type>>()
                       ->setSelectedScale(scale);

            return *this;
        }

        /**
         * @brief Sets the action ID for this menu item.
         *
         * @param actionId The action to trigger when activated.
         *
         * @return Reference to this config for method chaining.
         */
        MenuItemConfig& actionId(const helios::ui::widgets::types::ActionId actionId) {

            gameObject_.template add<helios::ui::widgets::components::UiActionComponent<Handle_type>>(actionId);

            return *this;
        }

        /**
         * @brief Sets the index position in the parent menu.
         *
         * @param index The index position.
         *
         * @return Reference to this config for method chaining.
         */
        MenuItemConfig& index(const size_t index) {

            parentMenu_->template get<helios::ui::widgets::components::MenuComponent<Handle_type>>()
                       ->insert(gameObject_.handle(), index);

            if (isSelected_) {
                parentMenu_->template get<helios::ui::widgets::components::MenuComponent<Handle_type>>()
                       ->setSelectedIndex(index);
            }

            index_ = index;

            return *this;
        }


    };

}
