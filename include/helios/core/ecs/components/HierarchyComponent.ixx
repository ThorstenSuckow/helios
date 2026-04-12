/**
 * @file HierarchyComponent.ixx
 * @brief Component for parent-child entity relationships.
 */
module;

#include <optional>
#include <span>
#include <vector>

export module helios.core.ecs.components.HierarchyComponent;

import helios.core.ecs.EntityHandle;

export namespace helios::core::ecs::components {

    /**
     * @brief Stores parent-child relationships for hierarchical entity graphs.
     *
     * @details HierarchyComponent enables entities to form tree structures
     * where state changes (e.g., activation, transformation) can propagate
     * from parent to child entities. The dirty flag signals that the
     * hierarchy has changed and needs propagation.
     *
     * @tparam THandle The entity handle type used for parent/child references.
     */
    template<typename THandle>
    class HierarchyComponent  {

        /**
         * @brief Child entity handles.
         */
        std::vector<THandle> children_;

        /**
         * @brief Optional parent entity handle.
         */
        std::optional<THandle> parent_;

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
        void addChild(const THandle child) {
            children_.push_back(child);
        }

        /**
         * @brief Sets the parent entity for this hierarchy node.
         *
         * @param parent Handle of the parent entity.
         */
        void setParent(const THandle parent) {
            parent_ = parent;
        }

        /**
         * @brief Returns the parent entity handle if set.
         *
         * @return Optional containing the parent handle, or std::nullopt.
         */
        [[nodiscard]] std::optional<THandle> parent() const noexcept {
            return parent_;
        }

        /**
         * @brief Returns a span over all child entity handles.
         *
         * @return Read-only span of child handles.
         */
        [[nodiscard]] std::span<const THandle> children() noexcept {
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