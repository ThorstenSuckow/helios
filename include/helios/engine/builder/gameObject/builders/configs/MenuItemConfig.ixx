/**
 * @file MenuItemConfig.ixx
 * @brief Fluent configuration for menu item GameObjects.
 */
module;

#include <cassert>
#include <optional>

export module helios.engine.builder.gameObject.builders.configs.MenuItemConfig;

import helios.engine.ecs.GameObject;

import helios.engine.core.data;

import helios.engine.modules.ui.widgets.components.MenuComponent;
import helios.engine.modules.ui.widgets.components.UiStyleComponent;
import helios.engine.modules.ui.widgets.components.UiStateComponent;
import helios.engine.modules.ui.widgets.components.UiActionComponent;

import helios.math;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for menu item GameObjects.
     *
     * Provides a builder-style interface for configuring menu items including
     * styling, selection state, and action binding. Automatically attaches
     * the item to the parent menu.
     */
    class MenuItemConfig {

        /**
         * @brief The GameObject being configured.
         */
        helios::engine::ecs::GameObject gameObject_;

        /**
         * @brief Pointer to the parent menu GameObject.
         */
        helios::engine::ecs::GameObject* parentMenu_ = nullptr;

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

            auto* mc = parentMenu_->get<helios::engine::modules::ui::widgets::components::MenuComponent>();

            assert(mc != nullptr && "Unexpected nullptr for MenuComponent");

            mc->addMenuItem(gameObject_);
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
            helios::engine::ecs::GameObject gameObject,
            helios::engine::ecs::GameObject& parentMenu
        ) : gameObject_(gameObject), parentMenu_(&parentMenu) {


            gameObject_.getOrAdd<helios::engine::modules::ui::widgets::components::UiStyleComponent>();
            gameObject_.getOrAdd<helios::engine::modules::ui::widgets::components::UiStateComponent>();


            assert(parentMenu_->has<helios::engine::modules::ui::widgets::components::MenuComponent>() &&
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
                parentMenu_->get<helios::engine::modules::ui::widgets::components::MenuComponent>()
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
            gameObject_.get<helios::engine::modules::ui::widgets::components::UiStyleComponent>()
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
            gameObject_.get<helios::engine::modules::ui::widgets::components::UiStyleComponent>()
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
            gameObject_.get<helios::engine::modules::ui::widgets::components::UiStyleComponent>()
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
            gameObject_.get<helios::engine::modules::ui::widgets::components::UiStyleComponent>()
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
        MenuItemConfig& actionId(const helios::engine::core::data::ActionId actionId) {

            gameObject_.add<helios::engine::modules::ui::widgets::components::UiActionComponent>(actionId);

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

            parentMenu_->get<helios::engine::modules::ui::widgets::components::MenuComponent>()
                       ->insert(gameObject_, index);

            if (isSelected_) {
                parentMenu_->get<helios::engine::modules::ui::widgets::components::MenuComponent>()
                       ->setSelectedIndex(index);
            }

            index_ = index;

            return *this;
        }


    };

}
