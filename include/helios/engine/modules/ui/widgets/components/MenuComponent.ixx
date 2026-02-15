/**
 * @file MenuComponent.ixx
 * @brief Component for managing UI menu structures.
 */
module;

#include <span>
#include <vector>

export module helios.engine.modules.ui.widgets.components.MenuComponent;

import helios.engine.core.data.MenuId;
import helios.engine.ecs.GameObject;
import helios.engine.ecs.EntityHandle;

export namespace helios::engine::modules::ui::widgets::components {

/**
 * @class MenuComponent
 * @brief Manages a collection of menu items for UI navigation.
 *
 * Stores entity handles to menu items and tracks the currently
 * selected item index.
 */
class MenuComponent {

    std::vector<helios::engine::ecs::EntityHandle> menuItems_;
    helios::engine::core::data::MenuId menuId_{};
    size_t selectedIndex_ = 0;
    size_t previousSelectedIndex_ = 0;


public:

    /**
     * @brief Sets the menu identifier.
     *
     * @param id The menu ID.
     */
    void setMenuId(helios::engine::core::data::MenuId id) noexcept {
        menuId_ = id;
    }

    /**
     * @brief Returns the menu identifier.
     */
    [[nodiscard]] helios::engine::core::data::MenuId menuId() const noexcept {
        return menuId_;
    }

    /**
     * @brief Sets the currently selected item index.
     *
     * @param index The index to select.
     */
    void setSelectedIndex(const size_t index) noexcept {
        selectedIndex_ = index;
    }

    /**
     * @brief Returns the currently selected item index.
     */
    [[nodiscard]] size_t selectedIndex() const noexcept {
        return selectedIndex_;
    }

    /**
     * @brief Sets the previously selected item index.
     *
     * @param index The previous index.
     */
    void setPreviousSelectedIndex(const size_t index) noexcept {
        previousSelectedIndex_ = index;
    }

    /**
     * @brief Returns the previously selected item index.
     */
    [[nodiscard]] size_t previousSelectedIndex() const noexcept {
        return previousSelectedIndex_;
    }

    /**
     * @brief Adds a menu item to the end of the list.
     *
     * @param menuItem The GameObject to add as a menu item.
     */
    void addMenuItem(const helios::engine::ecs::GameObject menuItem) {
        menuItems_.push_back(menuItem.entityHandle());
    }

    /**
     * @brief Returns a span of all menu item handles.
     */
    [[nodiscard]] std::span<helios::engine::ecs::EntityHandle> menuItems() noexcept {
        return menuItems_;
    }

    /**
     * @brief Inserts a menu item at a specific index.
     *
     * @param menuItem The GameObject to insert.
     * @param index The index at which to insert.
     */
    void insert(const helios::engine::ecs::GameObject menuItem, const size_t index) {
        if (index >= menuItems_.size()) {
            menuItems_.resize(index + 1);
        }

        for (size_t i = 0; i < menuItems_.size(); i++) {
            if (i == index && menuItems_[i] == menuItem.entityHandle()) {
                return;
            }
        }

        const auto old = menuItems_[index];
        menuItems_[index] = menuItem.entityHandle();
        menuItems_.push_back(old);
    }
};

} // namespace helios::engine::modules::ui::widgets::components
