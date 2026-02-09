/**
 * @file EntityManager.ixx
 * @brief Central manager for entity lifecycle and component storage.
 */
module;

#include <cassert>
#include <generator>
#include <memory>
#include <ranges>
#include <vector>
#include <helios/helios_config.h>

export module helios.engine.ecs.EntityManager;

import helios.engine.core.data;
import helios.core.data;
import helios.engine.ecs.SparseSet;
import helios.engine.ecs.Traits;
import helios.engine.ecs.EntityRegistry;
import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.ComponentOps;
import helios.engine.ecs.ComponentOpsRegistry;

namespace  {
    }

export namespace helios::engine::ecs {
    using namespace helios::engine::ecs;

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
         *
         * @odo sort after size()
         */
        std::vector<std::unique_ptr<SparseSetBase>> components_;

        /**
         * @brief Reference to the entity registry for handle management.
         */
        EntityRegistry& registry_;

        /**
         * @brief initial capacity for the underlying sparsesets.
         */
        const size_t capacity_;

    public:

        /**
         * @brief Constructs an EntityManager with the given registry.
         *
         * @param registry The EntityRegistry used for handle allocation and validation.
         */
        explicit EntityManager(EntityRegistry& registry, size_t capacity = ENTITY_MANAGER_DEFAULT_CAPACITY)
        : registry_(registry), capacity_(capacity) {}

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
         * @brief Checks if an entity handle is valid.
         *
         * @param handle The handle to validate.
         *
         * @return `true` if the handle refers to a living entity.
         */
        [[nodiscard]] bool isValid(const EntityHandle handle) const noexcept {
            return registry_.isValid(handle);
        }

        /**
         * @brief Checks if an entity ID is valid.
         *
         * @param entityId The entity ID to validate.
         *
         * @return `true` if the entity ID refers to a living entity.
         */
        [[nodiscard]] bool isValid(const EntityId entityId) const noexcept {
            return registry_.isValid(handle(entityId));
        }

        /**
         * @brief Destroys an entity and invalidates its handle.
         *
         * @details Increments the entity's version in the registry, making all
         * existing handles to this entity stale. Does not automatically remove
         * components from storage.
         *
         * @param handle The handle of the entity to destroy.
         *
         * @return `true` if the entity was destroyed, `false` if already invalid.
         */
        [[nodiscard]] bool destroy(const EntityHandle& handle) {

            if (!registry_.isValid(handle)) {
                return false;
            }

            for (size_t i = 0; i < components_.size(); i++) {

                if (!components_[i]) {
                    continue;
                }
                const auto typeId = ComponentTypeId{i};
                if (void* rawCmp = raw(handle, typeId)) {
                    if (const auto& ops = helios::engine::ecs::ComponentOpsRegistry::ops(typeId); ops.onRemove) {
                        ops.onRemove(rawCmp);
                    }
                }

                components_[i]->remove(handle.entityId);
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
        template<typename T>
        [[nodiscard]] T* get(const EntityHandle handle) const {
            if (!has<T>(handle)) {
                return nullptr;
            }

            const auto entityId = handle.entityId;
            const auto typeId = ComponentTypeId::id<T>().value();

            auto* sparseSet = static_cast<SparseSet<T>*>(components_[typeId].get());

            return sparseSet->get(entityId);
        }

        /**
         * @brief Returns the SparseSet for a component type.
         *
         * @tparam T The component type.
         *
         * @return Pointer to the SparseSet, or `nullptr` if the type has no storage.
         */
        template<typename T>
        [[nodiscard]] SparseSet<T>* getSparseSet() {

            const auto typeId = ComponentTypeId::id<T>().value();

            if (typeId >= components_.size()) {
                return nullptr;
            }

            return static_cast<SparseSet<T>*>(components_[typeId].get());
        }

        /**
         * @brief Returns the SparseSet for a component type (const).
         *
         * @tparam T The component type.
         *
         * @return Const pointer to the SparseSet, or `nullptr` if the type has no storage.
         */
        template<typename T>
        [[nodiscard]] const SparseSet<T>* getSparseSet() const {

            const auto typeId = ComponentTypeId::id<T>().value();

            if (typeId >= components_.size()) {
                return nullptr;
            }

            return static_cast<SparseSet<T>*>(components_[typeId].get());
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
        template<typename T>
        [[nodiscard]] bool has(const EntityHandle handle) const {
            if (!registry_.isValid(handle)) {
                return false;
            }

            const auto typeId = ComponentTypeId::id<T>().value();

            if (typeId < components_.size() && components_[typeId]) {
                return components_[typeId]->contains(handle.entityId);
            }

            return false;
        }

        /**
         * @brief Checks whether an entity has a component by type ID.
         *
         * @param handle The entity to query.
         * @param typeId The component type identifier.
         *
         * @return `true` if the entity has the component, `false` otherwise.
         */
        [[nodiscard]] bool has(const EntityHandle handle, const helios::engine::core::data::ComponentTypeId typeId) const {
            if (!registry_.isValid(handle)) {
                return false;
            }

            const auto tvalue = typeId.value();

            if (tvalue < components_.size() && components_[tvalue]) {
                return components_[tvalue]->contains(handle.entityId);
            }

            return false;
        }

        /**
         * @brief Enables a component by type ID.
         *
         * @details Calls the component's `enable()` method if registered.
         *
         * @param handle The entity whose component to enable.
         * @param typeId The component type identifier.
         */
        void enable(const EntityHandle handle, const helios::engine::core::data::ComponentTypeId typeId) const {
            enable(handle, typeId, true);
        }

        /**
         * @brief Disables a component by type ID.
         *
         * @details Calls the component's `disable()` method if registered.
         *
         * @param handle The entity whose component to disable.
         * @param typeId The component type identifier.
         */
        void disable(const EntityHandle handle, const helios::engine::core::data::ComponentTypeId typeId) const {
            enable(handle, typeId, false);
        }

        /**
         * @brief Enables or disables a component by type ID.
         *
         * @param handle The entity whose component to modify.
         * @param typeId The component type identifier.
         * @param enable `true` to enable, `false` to disable.
         */
        void enable(const EntityHandle handle, const helios::engine::core::data::ComponentTypeId typeId, const bool enable) const {

            if (!has(handle, typeId)) {
                return;
            }

            void* rawCmp = raw(handle, typeId);
            if (!rawCmp) {
                return;
            }
            const auto& ops = helios::engine::ecs::ComponentOpsRegistry::ops(typeId);

            if (enable && ops.enable) {
                ops.enable(rawCmp);
            } else if (!enable && ops.disable) {
                ops.disable(rawCmp);
            }
        }

        /**
         * @brief Constructs and attaches a component to an entity.
         *
         * If the component type has not been registered yet, a new `SparseSet`
         * is created. The component is constructed in-place with the provided
         * arguments. Return nullptre if the component already exists or if the
         * handle was invalid.
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

            const auto typeId = ComponentTypeId::id<T>().value();

            if (typeId >= components_.size()) {
                components_.resize(typeId + 1);
            }

            if (!components_[typeId]) {
                components_[typeId] = std::make_unique<SparseSet<T>>(capacity_);
            }

            auto* sparseSet = static_cast<SparseSet<T>*>(components_[typeId].get());

            if (sparseSet->contains(entityId)) {
                return nullptr;
            }

            return sparseSet->emplace(entityId, std::forward<Args>(args)...);
        }

        /**
         * @brief Returns existing component or creates a new one.
         *
         * @tparam T The component type.
         * @tparam Args Constructor argument types.
         *
         * @param handle The entity.
         * @param args Arguments forwarded to the constructor if creating.
         *
         * @return Pointer to the existing or newly created component,
         *         or `nullptr` if the handle is invalid.
         */
        template<typename T, typename... Args>
        T* emplaceOrGet(const EntityHandle handle, Args&& ...args) {

            if (!registry_.isValid(handle)) {
                return nullptr;
            }

            auto* raw = emplace<T>(handle, std::forward<Args>(args)...);

            if (!raw) {
                return get<T>(handle);
            }

            return nullptr;
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

            if (!has<T>(handle)) {
                return false;
            }

            const auto typeId = ComponentTypeId::id<T>();
            void* rawCmp = raw(handle, typeId);
            if (!rawCmp) {
                return false;
            }
            const auto& ops = helios::engine::ecs::ComponentOpsRegistry::ops(typeId);

            if (ops.onRemove && !ops.onRemove(rawCmp)) {
                return false;
            }

            return components_[typeId.value()]->remove(handle.entityId);
        }

        /**
         * @brief Returns a generator over all component type IDs attached to an entity.
         *
         * @param handle The entity to query.
         *
         * @return Generator yielding ComponentTypeId for each attached component.
         */
        std::generator<helios::engine::core::data::ComponentTypeId>
        componentTypeIds(const EntityHandle handle) const {
            if (!registry_.isValid(handle)) {
                co_return;
            }

            for (size_t i = 0; i < components_.size(); i++) {
                if (components_[i] && components_[i]->contains(handle.entityId)) {
                    co_yield ComponentTypeId{i};
                }
            }
        }

        /**
         * @brief Clones all components from source entity to target entity.
         *
         * @details Iterates through all components on the source and copies them
         * to the target using the registered clone function. Skips components
         * that already exist on the target.
         *
         * @param source The entity to clone from.
         * @param target The entity to clone to.
         */
        void clone(const EntityHandle source, const EntityHandle target) {

            if (!registry_.isValid(source)) {
                return;
            }

            for (auto typeId : componentTypeIds(source)) {

                if (!has(target, typeId)) {

                    const auto& ops = ComponentOpsRegistry::ops(typeId);

                    const void* sourceCmp = raw(source, typeId);

                    if (sourceCmp && ops.clone) {
                        ops.clone(this, sourceCmp, &target);
                    }

                }

            }
        }

        /**
         * @brief Returns raw void pointer to a component.
         *
         * @param handle The entity.
         * @param typeId The component type identifier.
         *
         * @return Raw pointer to the component, or `nullptr` if not found.
         */
        [[nodiscard]] void* raw(const EntityHandle handle, const helios::engine::core::data::ComponentTypeId typeId ) const {
            if (!has(handle, typeId)) {
                return nullptr;
            }

            return components_[typeId.value()]->raw(handle.entityId);
        }

        /**
         * @brief Reconstructs an EntityHandle from an EntityId.
         *
         * @param entityId The entity ID.
         *
         * @return EntityHandle with current version from the registry.
         */
        [[nodiscard]] EntityHandle handle(const EntityId entityId) const {
            return EntityHandle{entityId, registry_.version(entityId)};
        }


    };


}