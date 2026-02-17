/**
 * @file HierarchyComponent.ixx
 * @brief Component for parent-child entity relationships.
 */
module;

#include <optional>
#include <span>
#include <vector>

export module helios.engine.ecs.components.HierarchyComponent;

import helios.engine.ecs.EntityHandle;

export namespace helios::engine::ecs::components {

    /**
     * @brief Stores parent-child relationships for hierarchical entity graphs.
     *
     * @details HierarchyComponent enables entities to form tree structures
     * where state changes (e.g., activation, transformation) can propagate
     * from parent to child entities. The dirty flag signals that the
     * hierarchy has changed and needs propagation.
     */
    class HierarchyComponent  {

        /**
         * @brief Child entity handles.
         */
        std::vector<helios::engine::ecs::EntityHandle> children_;

        /**
         * @brief Optional parent entity handle.
         */
        std::optional<helios::engine::ecs::EntityHandle> parent_;

        /**
         * @brief Dirty flag indicating pending hierarchy updates.
         */
        bool isDirty_ = false;

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

    public:

        /**
         * @brief Adds a child entity to this hierarchy node.
         *
         * @param child Handle of the child entity to add.
         */
        void addChild(const helios::engine::ecs::EntityHandle child) {
            children_.push_back(child);
        }

        /**
         * @brief Sets the parent entity for this hierarchy node.
         *
         * @param parent Handle of the parent entity.
         */
        void setParent(const helios::engine::ecs::EntityHandle parent) {
            parent_ = parent;
        }

        /**
         * @brief Returns the parent entity handle if set.
         *
         * @return Optional containing the parent handle, or std::nullopt.
         */
        [[nodiscard]] std::optional<helios::engine::ecs::EntityHandle> parent() const noexcept {
            return parent_;
        }

        /**
         * @brief Returns a span over all child entity handles.
         *
         * @return Read-only span of child handles.
         */
        [[nodiscard]] std::span<const helios::engine::ecs::EntityHandle> children() noexcept {
            return children_;
        }

        /**
         * @brief Marks the hierarchy as requiring propagation.
         */
        void markDirty() {
            isDirty_ = true;
        }

        /**
         * @brief Clears the dirty flag after propagation.
         */
        void clearDirty() {
            isDirty_ = false;
        }

        /**
         * @brief Checks whether the hierarchy needs propagation.
         *
         * @return True if dirty, false otherwise.
         */
        [[nodiscard]] bool isDirty() const noexcept {
            return isDirty_;
        }

    };


}