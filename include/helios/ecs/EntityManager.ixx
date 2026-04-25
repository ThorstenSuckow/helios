/**
 * @file EntityManager.ixx
 * @brief Central manager for entity lifecycle and component storage.
 */
module;

#include <memory>
#include <vector>
#include <cstddef>

export module helios.ecs.EntityManager;

import helios.ecs.SparseSet;
import helios.ecs.EntityRegistry;
import helios.ecs.ComponentOpsRegistry;

import helios.ecs.types;

import helios.ecs.concepts.Traits;


using namespace helios::ecs::types;
using namespace helios::ecs::concepts;
export namespace helios::ecs {
    
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
     * - **Ownership Semantics:** Copy construction/assignment are deleted;
     *   `EntityManager` is move-enabled only.
     *
     * ## Usage
     *
     * ```cpp
     * EntityManager<MyHandle, MyRegistry, 1024> manager;
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
     * @tparam THandle       Handle type used to identify entities (e.g. `EntityHandle<GameDomainTag>`).
     * @tparam TEntityRegistry Registry type that manages handle allocation and versioning.
     * @tparam TCapacity     Default initial capacity for the registry and sparse sets.
     *
     * @see EntityRegistry
     * @see SparseSet
     * @see EntityHandle
     * @see TypedHandleWorld
     */
    template<
        typename THandle, 
        typename TEntityRegistry,
        size_t TCapacity
    > 
    class EntityManager {


    public:

        using EntityRegistry_type = TEntityRegistry;
        using Handle_type = THandle;
        using StrongId_type = Handle_type::StrongId_type;
        using ComponentTypeId_type = ComponentTypeId<Handle_type>;
        using ComponentOpsRegistry_type = ComponentOpsRegistry<Handle_type>;

        /**
         * @brief Non-copyable: copying an EntityManager is explicitly disabled.
         */
        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;

        EntityManager(EntityManager&&) noexcept = default;
        EntityManager& operator=(EntityManager&&) noexcept = default;

        /**
         * @brief Constructs an EntityManager with the given capacity.
         *
         * Creates an internally owned `EntityRegistry` initialized with the
         * specified capacity.
         *
         * @param capacity Initial capacity for the registry and sparse sets.
         *                 Defaults to `TCapacity`.
         */
        explicit EntityManager(const size_t capacity = TCapacity)
        : registry_(EntityRegistry_type{capacity}), capacity_(capacity)
        {}

        /**
         * @brief Constructs an EntityManager with an external registry.
         *
         * @param registry External EntityRegistry to use for handle allocation.
         * @param capacity Initial capacity for the sparse sets.
         *                 Defaults to `TCapacity`.
         *
         * @deprecated Prefer the capacity-only constructor which creates its
         *             own registry internally.
         */
        explicit EntityManager(
            EntityRegistry_type& registry,
            const size_t capacity = TCapacity)
        : registry_(registry), capacity_(capacity)
        {}

        /**
         * @brief Creates a new entity.
         *
         * Delegates to the underlying `EntityRegistry` to allocate a new handle.
         *
         * @return A valid `EntityHandle` for the newly created entity.
         *
         * @see EntityRegistry::create
         */
        [[nodiscard]] Handle_type create(StrongId_type strongId = StrongId_type{}) {
            return registry_.create(strongId);
        }

        /**
         * @brief Checks if an entity handle is valid.
         *
         * @param handle The handle to validate.
         *
         * @return `true` if the handle refers to a living entity.
         */
        [[nodiscard]] bool isValid(const Handle_type handle) const noexcept {
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
         * existing handles to this entity stale. Does automatically remove
         * its components from storage.
         *
         * @param handle The handle of the entity to destroy.
         *
         * @return `true` if the entity was destroyed, `false` if already invalid.
         */
        [[nodiscard]] bool destroy(const Handle_type handle) {

            if (!registry_.isValid(handle)) {
                return false;
            }

            for (size_t i = 0; i < components_.size(); i++) {

                if (!components_[i]) {
                    continue;
                }
                const auto typeId = ComponentTypeId_type{i};
                if (void* rawCmp = raw(handle, typeId)) {
                    if (const auto& ops = ComponentOpsRegistry_type::ops(typeId); ops.onRemove) {
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
        [[nodiscard]] T* get(const Handle_type handle) const {
            if (!has<T>(handle)) {
                return nullptr;
            }

            const auto entityId = handle.entityId;
            const auto typeId = ComponentTypeId_type::template id<T>().value();

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

            const auto typeId = ComponentTypeId_type::template id<T>().value();

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

            const auto typeId = ComponentTypeId_type::template id<T>().value();

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
        [[nodiscard]] bool has(const Handle_type handle) const {
            if (!registry_.isValid(handle)) {
                return false;
            }

            const auto typeId = ComponentTypeId_type::template id<T>().value();

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
        [[nodiscard]] bool has(const Handle_type handle, 
            const ComponentTypeId_type typeId) const {
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
        void enable(const Handle_type handle, const ComponentTypeId_type typeId) const {
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
        void disable(const Handle_type handle, const ComponentTypeId_type typeId) const {
            enable(handle, typeId, false);
        }

        /**
         * @brief Enables or disables a component by type ID.
         *
         * @param handle The entity whose component to modify.
         * @param typeId The component type identifier.
         * @param enable `true` to enable, `false` to disable.
         */
        void enable(const Handle_type handle, const ComponentTypeId_type typeId, const bool enable) const {

            if (!has(handle, typeId)) {
                return;
            }

            void* rawCmp = raw(handle, typeId);
            if (!rawCmp) {
                return;
            }
            const auto& ops = ComponentOpsRegistry_type::ops(typeId);

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
         * arguments. Returns `nullptr` if the component already exists or if the
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
        T* emplace(const Handle_type handle, Args&& ...args) {

            if (!registry_.isValid(handle)) {
                return nullptr;
            }

            const auto entityId = handle.entityId;

            const auto typeId = ComponentTypeId_type::template id<T>().value();

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
        T* emplaceOrGet(const Handle_type handle, Args&& ...args) {

            if (!registry_.isValid(handle)) {
                return nullptr;
            }

            auto* raw = emplace<T>(handle, std::forward<Args>(args)...);

            if (!raw) {
                return get<T>(handle);
            }

            return raw;
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
        bool remove(const Handle_type& handle) {

            if (!has<T>(handle)) {
                return false;
            }

            const auto typeId = ComponentTypeId_type::template id<T>();
            void* rawCmp = raw(handle, typeId);
            if (!rawCmp) {
                return false;
            }
            const auto& ops = ComponentOpsRegistry_type::ops(typeId);

            if (ops.onRemove && !ops.onRemove(rawCmp)) {
                return false;
            }

            return components_[typeId.value()]->remove(handle.entityId);
        }

        /**
         * @brief Returns a span over all component type IDs attached to an entity.
         *
         * @param handle The entity to query.
         */
        template<typename TFunc>
        void forEachComponentTypeId(const Handle_type handle, TFunc&& func) const {
            if (!registry_.isValid(handle)) {
                return;
            }

            for (size_t i = 0; i < components_.size(); i++) {
                if (components_[i] && components_[i]->contains(handle.entityId)) {
                    std::forward<TFunc>(func)(ComponentTypeId_type{i});
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
        void clone(const Handle_type source, const Handle_type target) {

            if (!registry_.isValid(source)) {
                return;
            }

            forEachComponentTypeId(
                [&](const ComponentTypeId_type typeId) {
                    if (!has(target, typeId)) {

                        const auto& ops = ComponentOpsRegistry_type::ops(typeId);
                        const void* sourceCmp = raw(source, typeId);

                        if (sourceCmp && ops.clone) {
                            ops.clone(this, sourceCmp, &target);
                        }
                    }
                }
            );

        }

        /**
         * @brief Returns raw void pointer to a component.
         *
         * @param handle The entity.
         * @param typeId The component type identifier.
         *
         * @return Raw pointer to the component, or `nullptr` if not found.
         */
        [[nodiscard]] void* raw(const Handle_type handle, const ComponentTypeId_type typeId ) const {
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
        [[nodiscard]] Handle_type handle(const EntityId entityId) const {
            return Handle_type{entityId, registry_.version(entityId), registry_.strongId(entityId)};
        }

    private:
        /**
         * @brief Component storage indexed by type ID.
         *
         * @todo sort after size()
         */
        std::vector<std::unique_ptr<SparseSetBase>> components_;

        /**
         * @brief Entity registry owned by this EntityManager.
         */
        EntityRegistry_type registry_;

        /**
         * @brief Initial capacity for the underlying sparse sets.
         */
        const size_t capacity_;

    };


}