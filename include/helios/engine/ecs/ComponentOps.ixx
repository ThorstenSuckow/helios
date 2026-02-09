/**
 * @file ComponentOps.ixx
 * @brief Function pointer structure for type-erased component lifecycle callbacks.
 */
module;

export module helios.engine.ecs.ComponentOps;

import helios.engine.ecs.EntityHandle;

export namespace helios::engine::ecs {

    /**
     * @brief Container for type-erased component lifecycle function pointers.
     *
     * @details `ComponentOps` enables runtime invocation of component lifecycle
     * hooks without knowing the concrete component type. Function pointers are
     * set during type registration via `ComponentReflector::registerType<T>()`.
     *
     * ## Type Erasure Pattern
     *
     * ```cpp
     * // At registration (type known):
     * ComponentOps ops{
     *     .onAcquire = [](void* ptr) {
     *         static_cast<HealthComponent*>(ptr)->onAcquire();
     *     }
     * };
     *
     * // At runtime (type erased):
     * if (ops.onAcquire) {
     *     ops.onAcquire(rawComponent);
     * }
     * ```
     *
     * @see ComponentOpsRegistry
     * @see ComponentReflector
     * @see Traits
     */
    struct ComponentOps {

        /**
         * @brief Function signature for pool acquisition callback.
         *
         * @param ptr Raw pointer to the component instance.
         */
        using OnAcquireFn = void(*)(void*);

        /**
         * @brief Function signature for pool release callback.
         *
         * @param ptr Raw pointer to the component instance.
         */
        using OnReleaseFn = void(*)(void*);

        /**
         * @brief Function signature for removal interception callback.
         *
         * @param ptr Raw pointer to the component instance.
         *
         * @return `true` to allow removal, `false` to prevent it.
         */
        using OnRemoveFn = bool(*)(void*);

        /**
         * @brief Function signature for component enable callback.
         *
         * @param ptr Raw pointer to the component instance.
         */
        using EnableFn = void(*)(void*);

        /**
         * @brief Function signature for component disable callback.
         *
         * @param ptr Raw pointer to the component instance.
         */
        using DisableFn = void(*)(void*);

        /**
         * @brief Function signature for component cloning.
         *
         * @param manager Raw pointer to the EntityManager.
         * @param source Raw pointer to the source component.
         * @param target Pointer to the target EntityHandle.
         *
         * @return Raw pointer to the newly created component.
         */
        using CloneFn = void*(*)(void* manager, const void* source, const EntityHandle* target);

        /**
         * @brief Function signature for GameObject activation callback.
         *
         * @param ptr Raw pointer to the component instance.
         */
        using OnActivateFn = void(*)(void*);

        /**
         * @brief Function signature for GameObject deactivation callback.
         *
         * @param ptr Raw pointer to the component instance.
         */
        using OnDeactivateFn = void(*)(void*);

        /**
         * @brief Called when entity is acquired from an object pool.
         */
        OnAcquireFn onAcquire = nullptr;

        /**
         * @brief Called when entity is released back to an object pool.
         */
        OnReleaseFn onRelease = nullptr;

        /**
         * @brief Called before component removal. Return `false` to block.
         */
        OnRemoveFn onRemove = nullptr;

        /**
         * @brief Called to enable the component.
         */
        EnableFn enable = nullptr;

        /**
         * @brief Called to disable the component.
         */
        DisableFn disable = nullptr;

        /**
         * @brief Called to clone the component to a target entity.
         */
        CloneFn clone = nullptr;

        /**
         * @brief Called when the owning GameObject is activated.
         */
        OnActivateFn onActivate = nullptr;

        /**
         * @brief Called when the owning GameObject is deactivated.
         */
        OnDeactivateFn onDeactivate = nullptr;


    };

}