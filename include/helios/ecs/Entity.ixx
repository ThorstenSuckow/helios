/**
 * @file Entity.ixx
 * @brief High-level facade for entity manipulation in the ECS.
 */
module;

#include <cassert>
#include <type_traits>
#include <typeindex>


export module helios.ecs.Entity;


import helios.ecs.types.EntityHandle;

import helios.ecs.EntityManager;
import helios.ecs.components;

import helios.ecs.ComponentOpsRegistry;

import helios.ecs.types.ComponentTypeId;

using namespace helios::ecs::types;
export namespace helios::ecs {

    /**
     * @brief Lightweight facade for entity component manipulation.
     *
     * @details `Entity` provides a type-safe, convenient interface for
     * working with entities and their components. It wraps an `EntityHandle`
     * and a pointer to the `EntityManager`, delegating all operations.
     *
     * ## Size and Copy Semantics
     *
     * Entity is only 16 bytes (8 bytes EntityHandle + 8 bytes pointer) and
     * should be **passed by value**, not by reference:
     *
     * ```cpp
     * // Correct - pass by value
     * void processEntity(Entity entity) { ... }
     *
     * // Unnecessary - reference adds indirection
     * void processEntity(Entity& entity);       // Avoid
     * void processEntity(const Entity& entity); // Avoid
     * ```
     *
     * ## Hierarchy Integration
     *
     * When a Entity has a `HierarchyComponent`, activation changes trigger
     * hierarchy propagation. Calling `setActive()` marks the hierarchy as dirty,
     * and `HierarchyPropagationSystem` propagates the state to all descendants.
     *
     * ## Usage
     *
     * ```cpp
     * auto player = gameWorld.addEntity();
     *
     * // Add components
     * player.add<TransformComponent>(position);
     * player.add<HealthComponent>(100.0f);
     *
     * // Query and access
     * if (player.has<HealthComponent>()) {
     *     player.get<HealthComponent>()->takeDamage(10.0f);
     * }
     *
     * // Activation state (propagates to children if HierarchyComponent present)
     * player.setActive(false);  // Calls onDeactivate() on components
     * ```
     *
     * @tparam TEntityManager The entity manager type providing handle and component storage.
     *                        Must expose `Handle_type` and component operations.
     *
     * @see EntityHandle
     * @see EntityManager
     * @see GameWorld
     * @see HierarchyComponent
     * @see HierarchyPropagationSystem
     */
    template<typename TEntityManager>
    class Entity {

        /**
         * @brief The underlying entity identifier.
         */
        TEntityManager::Handle_type entityHandle_{0,0};

        /**
         * @brief Non-owning pointer to the EntityManager.
         */
        TEntityManager* entityManager_;


    public:

        using Handle_type = TEntityManager::Handle_type;
        using ComponentTypeId_type = ComponentTypeId<Handle_type>;
        using ComponentOpsRegistry_type = ComponentOpsRegistry<Handle_type>;
        using HierarchyComponent_type = components::HierarchyComponent<Handle_type>;
        using ActiveComponent_type = components::Active<Handle_type>;
        
        /**
         * @brief Constructs a Entity wrapper.
         *
         * @param entityHandle The entity handle to wrap.
         * @param entityManager Pointer to the EntityManager. Must not be null.
         */
        explicit Entity(
            const Handle_type entityHandle,
            TEntityManager* entityManager

        ) noexcept : entityHandle_(entityHandle), entityManager_(entityManager) {
            assert(entityManager_ != nullptr && "EntityManager must not be null.");
        };

        /**
         * @brief Returns a generator over all component type IDs attached to this entity.
         *
         * @return Generator yielding ComponentTypeId for each attached component.
         */
        template<typename TFunc>
        void forEachComponentTypeId(TFunc&& func) const {
            entityManager_->forEachComponentTypeId(entityHandle_, std::forward<TFunc>(func));
        }

        /**
         * @brief Returns the underlying entity handle.
         *
         * @return The EntityHandle for this Entity.
         */
        [[nodiscard]] Handle_type handle() noexcept {
            return entityHandle_;
        }

        /**
         * @brief Returns the underlying entity handle (const).
         *
         * @return The EntityHandle for this Entity.
         */
        [[nodiscard]] Handle_type handle() const noexcept {
            return entityHandle_;
        }

        /**
         * @brief Constructs and attaches a component to this entity.
         *
         * @details If the entity is active, calls `onActivate()` on the new component.
         * If inactive, calls `onDeactivate()`.
         *
         * @tparam T The component type to add.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the component constructor.
         *
         * @return Reference to the newly created component.
         */
        template<typename T, typename ...Args>
        T& add(Args&& ...args) {

            bool active = true;

            if (entityManager_->template  has<ActiveComponent_type>(entityHandle_)) {
                active = true;
            } else {
                active = false;
            }

            auto* cmp = entityManager_->template  emplace<T>(entityHandle_, std::forward<Args>(args)...);

            auto typeId = ComponentTypeId_type::template id<T>();
            const auto ops = ComponentOpsRegistry_type::ops(typeId);
            void* raw = entityManager_->raw(entityHandle_, typeId);

            if (active && ops.onActivate) {
                ops.onActivate(raw);
            } else if (!active && ops.onDeactivate) {
                ops.onDeactivate(raw);
            }

            return *cmp;
        }

        /**
         * @brief Returns existing component or creates a new one.
         *
         * @tparam T The component type.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the constructor if creating.
         *
         * @return Reference to the existing or newly created component.
         */
        template<typename T, typename ...Args>
        T& getOrAdd(Args&& ...args) {
            if (entityManager_->template  has<T>(entityHandle_)) {
                return *entityManager_->template  get<T>(entityHandle_);
            }
            return add<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Removes a component from this entity.
         *
         * @tparam T The component type to remove.
         *
         * @return True if the component was removed, false if not present.
         */
        template<typename T>
        bool remove() {
            return entityManager_->template  remove<T>(entityHandle_);
        }

        /**
         * @brief Returns raw pointer to component by type ID.
         *
         * @param typeId The component type identifier.
         *
         * @return Raw void pointer to the component, or nullptr if not found.
         */
        void* raw(const ComponentTypeId_type typeId) {
            return entityManager_->raw(entityHandle_, typeId);
        }

        /**
         * @brief Returns pointer to a component.
         *
         * @tparam T The component type.
         *
         * @return Pointer to the component, or nullptr if not attached.
         */
        template<typename T>
        T* get() {
            return entityManager_->template  get<T>(entityHandle_);
        }

        /**
         * @brief Returns const pointer to a component.
         *
         * @tparam T The component type.
         *
         * @return Const pointer to the component, or nullptr if not attached.
         */
        template<typename T>
        const T* get() const {
            return entityManager_->template  get<T>(entityHandle_);
        }

        /**
         * @brief Checks if this entity has a specific component type.
         *
         * @tparam T The component type to check.
         *
         * @return True if the component is attached, false otherwise.
         */
        template<typename T>
        bool has() const noexcept{
            return entityManager_->template  has<T>(entityHandle_);
        }

        /**
         * @brief Checks if this entity has a component by type ID.
         *
         * @param typeId The component type identifier.
         *
         * @return True if the component is attached, false otherwise.
         */
        bool has(ComponentTypeId_type typeId) const noexcept {
            return entityManager_->has(entityHandle_, typeId);
        }

        /**
         * @brief Enables a specific component by type ID.
         *
         * @param typeId The component type identifier.
         */
        void enableComponent(const ComponentTypeId_type typeId) {
            entityManager_->enable(entityHandle_, typeId);
        }

        /**
         * @brief Disables a specific component by type ID.
         *
         * @param typeId The component type identifier.
         */
        void disableComponent(const ComponentTypeId_type typeId) {
            entityManager_->disable(entityHandle_, typeId);
        }

        /**
         * @brief Sets the activation state of this Entity.
         *
         * @details When deactivated:
         * - The `Active` tag component is removed
         * - `onDeactivate()` is called on components that support it
         * - If a `HierarchyComponent` is present, it is marked dirty for propagation
         *
         * When activated:
         * - The `Inactive` tag component is removed
         * - An `Active` tag component is added
         * - `onActivate()` is called on components that support it
         * - If a `HierarchyComponent` is present, it is marked dirty for propagation
         *
         * @note Does **not** call `enable()`/`disable()` on components.
         *
         * @param active True to activate, false to deactivate.
         *
         * @see HierarchyComponent_type
         * @see HierarchyPropagationSystem
         */
        void setActive(const bool active) {
            bool isActive = entityManager_->template has<ActiveComponent_type>(entityHandle_);
            bool isInActive = !isActive;

            if (!isActive && active) {
                auto* hc =  entityManager_->template get<HierarchyComponent_type>(entityHandle_);
                if (hc) {
                    hc->markDirty();
                }

                entityManager_->template emplaceOrGet<ActiveComponent_type>(entityHandle_);
            }

            if (!isInActive && !active) {
                auto* hc = entityManager_->template get<HierarchyComponent_type>(entityHandle_);
                if (hc) {
                    hc->markDirty();
                }

                entityManager_->template  remove<ActiveComponent_type>(entityHandle_);
            }

            forEachComponentTypeId(
                [&](const ComponentTypeId_type typeId) {
                    const auto& ops = ComponentOpsRegistry_type::ops(typeId);

                    if (active && ops.onActivate) {
                        void* raw = entityManager_->raw(entityHandle_, typeId);
                        ops.onActivate(raw);
                    } else if (!active && ops.onDeactivate) {
                        void* raw = entityManager_->raw(entityHandle_, typeId);
                        ops.onDeactivate(raw);
                    }
                }
            );
        }

        /**
         * @brief Returns whether this Entity is active.
         *
         * @return True if the entity has the Active tag component.
         */
        [[nodiscard]] bool isActive() const {
            return entityManager_->template has<ActiveComponent_type>(entityHandle_);
        }

        /**
         * @brief Notifies all components that the entity is being released to a pool.
         *
         * @details Iterates through all attached components and calls `onRelease()`
         * on those that implement it.
         */
        void onRelease() {

            forEachComponentTypeId(
                [&](const ComponentTypeId_type typeId) {
                    const auto& ops = ComponentOpsRegistry_type::ops(typeId);

                    if (ops.onRelease) {
                        void* raw = entityManager_->raw(entityHandle_, typeId);
                        ops.onRelease(raw);
                    }
                }
            );


        }

        /**
         * @brief Notifies all components that the entity is being acquired from a pool.
         *
         * @details Iterates through all attached components and calls `onAcquire()`
         * on those that implement it.
         */
        void onAcquire() {
            forEachComponentTypeId(
                [&](const ComponentTypeId_type typeId) {
                    const auto& ops = ComponentOpsRegistry_type::ops(typeId);

                    if (ops.onAcquire) {
                        void* raw = entityManager_->raw(entityHandle_, typeId);
                        ops.onAcquire(raw);
                    }
                }
            );
        }

    };



} // namespace helios
