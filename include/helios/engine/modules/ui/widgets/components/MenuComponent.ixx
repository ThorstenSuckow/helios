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
 * @brief Manages a collection of menu items for UI navigation.
 *
 * @details Stores entity handles to menu items and tracks the currently
 * selected item index. Supports dirty tracking for efficient updates.
 *
 * @see MenuDisplaySystem
 * @see MenuNavigationSystem
 */
class MenuComponent {

    /**
     * @brief Entity handles of the menu items.
     */
    std::vector<helios::engine::ecs::EntityHandle> menuItems_;

    /**
     * @brief Unique identifier for this menu.
     */
    helios::engine::core::data::MenuId menuId_{};

    /**
     * @brief Currently selected item index.
     */
    size_t selectedIndex_ = 0;

    /**
     * @brief Previously selected item index.
     */
    size_t previousSelectedIndex_ = 0;

    /**
     * @brief Default selection index when menu is focused.
     */
    size_t defaultSelectedIndex_ = 0;

    /**
     * @brief Whether the menu state has changed.
     */
    bool isDirty_ = true;

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
     *
     * @return The MenuId for this menu.
     */
    [[nodiscard]] helios::engine::core::data::MenuId menuId() const noexcept {
        return menuId_;
    }

    /**
     * @brief Sets the currently selected item index.
     *
     * @details Marks the menu as dirty if the index changes.
     *
     * @param index The index to select.
     */
    void setSelectedIndex(const size_t index) noexcept {
        if (index != selectedIndex_) {
            markDirty();
        }
        previousSelectedIndex_ = selectedIndex_;
        selectedIndex_ = index;;
    }

    /**
     * @brief Resets selection to the default index.
     */
    void selectDefaultIndex() {
        setSelectedIndex(defaultSelectedIndex_);
    }

    /**
     * @brief Returns the currently selected item index.
     *
     * @return The selected index.
     */
    [[nodiscard]] size_t selectedIndex() const noexcept {
        return selectedIndex_;
    }


    /**
     * @brief Adds a menu item to the end of the list.
     *
     * @param menuItem The GameObject to add as a menu item.
     */
    void addMenuItem(const helios::engine::ecs::GameObject menuItem) {
        markDirty();
        menuItems_.push_back(menuItem.entityHandle());
    }

    /**
     * @brief Returns a span of all menu item handles.
     *
     * @return Span of EntityHandle for all menu items.
     */
    [[nodiscard]] std::span<helios::engine::ecs::EntityHandle> menuItems() noexcept {
        return menuItems_;
    }

    /**
     * @brief Clears the dirty flag.
     */
    void clearDirty() {
        isDirty_ = false;
    }

    /**
     * @brief Marks the menu as dirty.
     */
    void markDirty() {
        isDirty_ = true;
    }

    /**
     * @brief Returns whether the menu state has changed.
     *
     * @return True if dirty.
     */
    [[nodiscard]] bool isDirty() const noexcept {
        return isDirty_;
    }

    /**
     * @brief Returns the index of the previous selected menu index.
     *
     * @return The index of the previous selected menu item.
     */
    [[nodiscard]] size_t previousSelectedIndex() const noexcept {
        return previousSelectedIndex_;
    }

    /**
     * @brief Inserts a menu item at a specific index.
     *
     * @details Resizes the internal vector if necessary. If the item
     * already exists at the index, no action is taken.
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
