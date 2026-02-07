/**
 * @file EntityManager.ixx
 * @brief Central manager for entity lifecycle and component storage.
 */
module;

#include <memory>
#include <vector>

export module helios.engine.ecs.EntityManager;

import helios.engine.core.data;
import helios.core.data;
import helios.engine.ecs.SparseSet;
import helios.engine.ecs.EntityRegistry;
import helios.engine.ecs.EntityHandle;

namespace  {
    }

export namespace helios::engine::ecs {

    /**
     * @brief Internal tag type for component type indexing.
     * @internal
     */
    struct helios_engine_ecs_EntityManager_componentType {};


    using namespace helios::engine::ecs;

    /**
     * @brief Type indexer for component registration.
     */
    using TypeIndexer = helios::core::data::TypeIndexer<helios_engine_ecs_EntityManager_componentType>;


    /**
     * @brief Manages entities and their associated components.
     *
     * `EntityManager` provides a unified interface for creating entities and
     * attaching/retrieving components. It delegates handle management to an
     * `EntityRegistry` and stores component data in type-specific `SparseSet`
     * containers.
     *
     * ## Responsibilities
     *
     * - **Entity Creation:** Delegates to `EntityRegistry` for handle allocation.
     * - **Entity Destruction:** Removes all components and invalidates the handle.
     * - **Component Storage:** Maintains a vector of `SparseSet` instances, one per
     *   component type, indexed by `TypeIndexer`.
     *
     * ## Usage
     *
     * ```cpp
     * EntityRegistry registry;
     * EntityManager manager(registry);
     *
     * auto entity = manager.create();
     * auto* transform = manager.emplace<TransformComponent>(entity, glm::vec3{0.0f});
     *
     * if (manager.has<TransformComponent>(entity)) {
     *     auto* t = manager.get<TransformComponent>(entity);
     * }
     *
     * manager.remove<TransformComponent>(entity);  // Remove single component
     * manager.destroy(entity);                     // Destroy entity and all components
     * ```
     *
     * @see EntityRegistry
     * @see SparseSet
     * @see EntityHandle
     */
    class EntityManager {

        /**
         * @brief Component storage indexed by type ID.
         */
        std::vector<std::unique_ptr<SparseSetBase>> components_;

        /**
         * @brief Reference to the entity registry for handle management.
         */
        EntityRegistry& registry_;

    public:

        /**
         * @brief Constructs an EntityManager with the given registry.
         *
         * @param registry The EntityRegistry used for handle allocation and validation.
         */
        explicit EntityManager(EntityRegistry& registry)
        : registry_(registry) {}

        /**
         * @brief Creates a new entity.
         *
         * Delegates to the underlying `EntityRegistry` to allocate a new handle.
         *
         * @return A valid `EntityHandle` for the newly created entity.
         *
         * @see EntityRegistry::create
         */
        [[nodiscard]] EntityHandle create() {
            return registry_.create();
        }

        /**
         * @brief Destroys an entity and all its components.
         *
         * Iterates over all component storages and removes any data associated
         * with the given entity. If any component's `onRemove` trait returns
         * `false`, removal is cancelled and the entity remains valid.
         *
         * @param handle The entity to destroy.
         *
         * @return `true` if the entity was successfully destroyed, `false` if the
         *         handle was invalid or a component blocked removal.
         *
         * @see remove
         * @see SparseSet::remove
         * @see EntityRegistry::destroy
         */
        [[nodiscard]] bool destroy(const EntityHandle& handle) {

            if (!registry_.isValid(handle)) {
                return false;
            }

            const auto entityId = handle.entityId;

            // by default set to true. If no components are available, this
            // will never be set to false, so the handle can be removed in any case.
            bool canDestroy = true;
            for (auto& sparseSet : components_) {
                if (!sparseSet) {
                    // nullptr?
                    continue;
                }

                if (!sparseSet->contains(entityId)) {
                    continue;
                }
                if (!sparseSet->remove(entityId)) {
                    canDestroy = false;
                }
            }

            if (!canDestroy) {
                return false;
            }

            registry_.destroy(handle);
            return true;
        }

        /**
         * @brief Retrieves a component for the given entity.
         *
         * @tparam T The component type to retrieve.
         *
         * @param handle The entity whose component to retrieve.
         *
         * @return Pointer to the component, or `nullptr` if the entity is invalid
         *         or does not have the requested component.
         */
        template<typename T, typename... Args>
        [[nodiscard]] T* get(const EntityHandle handle) const {
            if (!registry_.isValid(handle)) {
                return nullptr;
            }

            const auto entityId = handle.entityId;
            const auto typeId = TypeIndexer::typeIndex<T>();

            if (typeId >= components_.size() || !components_[typeId]) {
                return nullptr;
            }

            auto* sparseSet = static_cast<SparseSet<T>*>(components_[typeId].get());

            return sparseSet->get(entityId);
        }

        /**
         * @brief Checks whether an entity has a specific component.
         *
         * @tparam T The component type to check for.
         *
         * @param handle The entity to query.
         *
         * @return `true` if the entity has the component, `false` if the handle
         *         is invalid or the component is not attached.
         */
        template<typename T, typename... Args>
        [[nodiscard]] bool has(const EntityHandle handle) const {
            if (!registry_.isValid(handle)) {
                return false;
            }

            const auto typeId = TypeIndexer::typeIndex<T>();

            if (typeId < components_.size() && components_[typeId] != nullptr) {
                return components_[typeId]->contains(handle.entityId);
            }

            return false;
        }

        /**
         * @brief Constructs and attaches a component to an entity.
         *
         * If the component type has not been registered yet, a new `SparseSet`
         * is created. The component is constructed in-place with the provided
         * arguments.
         *
         * @tparam T The component type to emplace.
         * @tparam Args Constructor argument types.
         *
         * @param handle The entity to attach the component to.
         * @param args Arguments forwarded to the component constructor.
         *
         * @return Pointer to the newly created component, or `nullptr` if the
         *         handle is invalid.
         */
        template<typename T, typename... Args>
        T* emplace(const EntityHandle handle, Args&& ...args) {

            if (!registry_.isValid(handle)) {
                return nullptr;
            }

            const auto entityId = handle.entityId;

            const auto typeId = TypeIndexer::typeIndex<T>();

            if (typeId >= components_.size()) {
                components_.resize(typeId + 1);
            }

            if (!components_[typeId]) {
                components_[typeId] = std::make_unique<SparseSet<T>>();
            }

            auto* sparseSet = static_cast<SparseSet<T>*>(components_[typeId].get());

            return sparseSet->emplace(entityId, std::forward<Args>(args)...);
        }

        /**
         * @brief Removes a specific component from an entity.
         *
         * Unlike `destroy()`, this only removes a single component type while
         * keeping the entity and other components intact.
         *
         * @tparam T The component type to remove.
         *
         * @param handle The entity whose component to remove.
         *
         * @return `true` if the component was removed, `false` if the handle was
         *         invalid, the component was not attached, or removal was blocked
         *         by `onRemove`.
         *
         * @see destroy
         * @see SparseSet::remove
         */
        template<typename T>
        bool remove(const EntityHandle& handle) {

            if (!registry_.isValid(handle)) {
                return false;
            }

            const auto entityId = handle.entityId;
            const auto typeId = TypeIndexer::typeIndex<T>();
            if (typeId >= components_.size() || !components_[typeId]) {
                return false;
            }

            return components_[typeId]->remove(entityId);
        }


    };


}