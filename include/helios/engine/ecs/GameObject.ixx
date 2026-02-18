/**
 * @file GameObject.ixx
 * @brief High-level facade for entity manipulation in the ECS.
 */
module;

#include <cassert>
#include <type_traits>
#include <typeindex>
#include <generator>


export module helios.engine.ecs.GameObject;


import helios.engine.ecs.EntityHandle;

import helios.engine.mechanics.lifecycle.components.Inactive;
import helios.engine.mechanics.lifecycle.components.Active;

import helios.util.Guid;

import helios.engine.ecs.EntityManager;
import helios.engine.ecs.components.HierarchyComponent;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.ecs.ComponentOpsRegistry;

import helios.engine.core.data.ComponentTypeId;
import helios.core.data;


export namespace helios::engine::ecs {

    /**
     * @brief Lightweight facade for entity component manipulation.
     *
     * @details `GameObject` provides a type-safe, convenient interface for
     * working with entities and their components. It wraps an `EntityHandle`
     * and a pointer to the `EntityManager`, delegating all operations.
     *
     * ## Size and Copy Semantics
     *
     * GameObject is only 16 bytes (8 bytes EntityHandle + 8 bytes pointer) and
     * should be **passed by value**, not by reference:
     *
     * ```cpp
     * // Correct - pass by value
     * void processEntity(GameObject entity) { ... }
     *
     * // Unnecessary - reference adds indirection
     * void processEntity(GameObject& entity);       // Avoid
     * void processEntity(const GameObject& entity); // Avoid
     * ```
     *
     * ## Hierarchy Integration
     *
     * When a GameObject has a `HierarchyComponent`, activation changes trigger
     * hierarchy propagation. Calling `setActive()` marks the hierarchy as dirty,
     * and `HierarchyPropagationSystem` propagates the state to all descendants.
     *
     * ## Usage
     *
     * ```cpp
     * auto player = gameWorld.addGameObject();
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
     * @see EntityHandle
     * @see EntityManager
     * @see GameWorld
     * @see HierarchyComponent
     * @see HierarchyPropagationSystem
     */
    class GameObject {

        /**
         * @brief The underlying entity identifier.
         */
        helios::engine::ecs::EntityHandle entityHandle_{0,0};

        /**
         * @brief Non-owning pointer to the EntityManager.
         */
        helios::engine::ecs::EntityManager* entityManager_;


    public:

        /**
         * @brief Constructs a GameObject wrapper.
         *
         * @param entityHandle The entity handle to wrap.
         * @param entityManager Pointer to the EntityManager. Must not be null.
         */
        explicit GameObject(
            const helios::engine::ecs::EntityHandle entityHandle,
            helios::engine::ecs::EntityManager* entityManager

        ) noexcept : entityHandle_(entityHandle), entityManager_(entityManager) {
            assert(entityManager_ != nullptr && "EntityManager must not be null.");
        };

        /**
         * @brief Returns a generator over all component type IDs attached to this entity.
         *
         * @return Generator yielding ComponentTypeId for each attached component.
         */
        [[nodiscard]] std::generator<helios::engine::core::data::ComponentTypeId>
        componentTypeIds() const {
            return entityManager_->componentTypeIds(entityHandle_);
        }

        /**
         * @brief Returns the underlying entity handle.
         *
         * @return The EntityHandle for this GameObject.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle entityHandle() noexcept {
            return entityHandle_;
        }

        /**
         * @brief Returns the underlying entity handle (const).
         *
         * @return The EntityHandle for this GameObject.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle entityHandle() const noexcept {
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

            if (entityManager_->has<helios::engine::mechanics::lifecycle::components::Active>(entityHandle_)) {
                active = true;
            } else {
                active = false;
            }

            auto* cmp = entityManager_->emplace<T>(entityHandle_, std::forward<Args>(args)...);

            auto typeId = helios::engine::core::data::ComponentTypeId::id<T>();
            const auto ops = helios::engine::ecs::ComponentOpsRegistry::ops(typeId);
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
            if (entityManager_->has<T>(entityHandle_)) {
                return *entityManager_->get<T>(entityHandle_);
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
        [[nodiscard]] bool remove() {
            return entityManager_->remove<T>(entityHandle_);
        }

        /**
         * @brief Returns raw pointer to component by type ID.
         *
         * @param typeId The component type identifier.
         *
         * @return Raw void pointer to the component, or nullptr if not found.
         */
        void* raw(const helios::engine::core::data::ComponentTypeId typeId) {
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
            return entityManager_->get<T>(entityHandle_);
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
            return entityManager_->get<T>(entityHandle_);
        }

        /**
         * @brief Checks if this entity has a specific component type.
         *
         * @tparam T The component type to check.
         *
         * @return True if the component is attached, false otherwise.
         */
        template<typename T>
        bool has() {
            return entityManager_->has<T>(entityHandle_);
        }

        /**
         * @brief Checks if this entity has a component by type ID.
         *
         * @param typeId The component type identifier.
         *
         * @return True if the component is attached, false otherwise.
         */
        bool has(helios::engine::core::data::ComponentTypeId typeId) {
            return entityManager_->has(entityHandle_, typeId);
        }

        /**
         * @brief Enables a specific component by type ID.
         *
         * @param typeId The component type identifier.
         */
        void enableComponent(const helios::engine::core::data::ComponentTypeId typeId) {
            entityManager_->enable(entityHandle_, typeId);
        }

        /**
         * @brief Disables a specific component by type ID.
         *
         * @param typeId The component type identifier.
         */
        void disableComponent(const helios::engine::core::data::ComponentTypeId typeId) {
            entityManager_->disable(entityHandle_, typeId);
        }

        /**
         * @brief Sets the activation state of this GameObject.
         *
         * @details When deactivated:
         * - An `Inactive` tag component is added
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
         * @see HierarchyComponent
         * @see HierarchyPropagationSystem
         */
        void setActive(const bool active) {

            bool isActive = entityManager_->has<helios::engine::mechanics::lifecycle::components::Active>(entityHandle_);
            bool isInActive = entityManager_->has<helios::engine::mechanics::lifecycle::components::Inactive>(entityHandle_);

            if (!isActive && active) {
                auto* hc =  entityManager_->get<helios::engine::ecs::components::HierarchyComponent>(entityHandle_);
                if (hc) {
                    hc->markDirty();
                }

                entityManager_->remove<helios::engine::mechanics::lifecycle::components::Inactive>(entityHandle_);
                entityManager_->emplaceOrGet<helios::engine::mechanics::lifecycle::components::Active>(entityHandle_);
            }

            if (!isInActive && !active) {
                auto* hc =  entityManager_->get<helios::engine::ecs::components::HierarchyComponent>(entityHandle_);
                if (hc) {
                    hc->markDirty();
                }

                entityManager_->emplaceOrGet<helios::engine::mechanics::lifecycle::components::Inactive>(entityHandle_);
                entityManager_->remove<helios::engine::mechanics::lifecycle::components::Active>(entityHandle_);
            }

            for (auto typeId : componentTypeIds()) {
                const auto ops = helios::engine::ecs::ComponentOpsRegistry::ops(typeId);
                void* raw = entityManager_->raw(entityHandle_, typeId);

                if (active && ops.onActivate) {
                    ops.onActivate(raw);
                } else if (!active && ops.onDeactivate) {
                    ops.onDeactivate(raw);
                }
            }
        }

        /**
         * @brief Returns whether this GameObject is active.
         *
         * @return True if the entity has the Active tag component.
         */
        [[nodiscard]] bool isActive() const {
            return entityManager_->has<helios::engine::mechanics::lifecycle::components::Active>(entityHandle_);
        }

        /**
         * @brief Notifies all components that the entity is being released to a pool.
         *
         * @details Iterates through all attached components and calls `onRelease()`
         * on those that implement it.
         */
        void onRelease() {
            for (auto typeId : componentTypeIds()) {
                const auto ops = helios::engine::ecs::ComponentOpsRegistry::ops(typeId);
                void* raw = entityManager_->raw(entityHandle_, typeId);

                if (ops.onRelease) {
                    ops.onRelease(raw);
                }
            }
        }

        /**
         * @brief Notifies all components that the entity is being acquired from a pool.
         *
         * @details Iterates through all attached components and calls `onAcquire()`
         * on those that implement it.
         */
        void onAcquire() {
            for (auto typeId : componentTypeIds()) {
                const auto ops = helios::engine::ecs::ComponentOpsRegistry::ops(typeId);
                void* raw = entityManager_->raw(entityHandle_, typeId);

                if (ops.onAcquire) {
                    ops.onAcquire(raw);
                }
            }
        }

    };



} // namespace helios::engine::modules
