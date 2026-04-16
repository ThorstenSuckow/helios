/**
 * @file ComponentReflector.ixx
 * @brief Compile-time reflection for component lifecycle hook registration.
 */
module;

#include <type_traits>

export module helios.ecs.ComponentReflector;

import helios.ecs.concepts.Traits;

import helios.ecs.types.ComponentTypeId;
import helios.ecs.types.EntityHandle;
import helios.ecs.types.ComponentOps;

import helios.ecs.ComponentOpsRegistry;
import helios.ecs.EntityManager;

using namespace helios::ecs::concepts::traits;
using namespace helios::ecs::types;
export namespace helios::ecs {

    /**
     * @brief Generates and registers ComponentOps for a component type.
     *
     * @details `ComponentReflector` uses compile-time trait detection to
     * generate appropriate function pointers for each lifecycle hook that
     * a component type implements. This enables zero-overhead conditional
     * invocation at runtime.
     *
     * ## Template Parameters
     *
     * The reflector is templated on `THandle` and `TEntityManager` so that
     * the generated clone function can cast the type-erased `void*` pointers
     * back to the concrete types of the target domain. This also ensures
     * that `ComponentOpsRegistry<THandle>` stores the ops in the correct
     * per-domain registry.
     *
     * ## Usage
     *
     * ```cpp
     * // Register a component type for the game domain
     * ComponentReflector<GameHandle, GameEM>::registerType<HealthComponent>();
     *
     * // Or use the module registry pattern
     * inline void registerComponents() {
     *     using R = ComponentReflector<GameHandle, GameEM>;
     *     R::registerType<CollisionComponent>();
     *     R::registerType<AabbColliderComponent>();
     * }
     * ```
     *
     * ## Trait Detection
     *
     * The reflector checks for the following traits at compile time:
     * - `HasOnAcquire` - `onAcquire()` method
     * - `HasOnRelease` - `onRelease()` method
     * - `HasOnRemove` - `onRemove()` method returning bool
     * - `HasToggleable` - `enable()` and `disable()` methods
     * - `HasClone` - `onClone(const T&)` method
     * - `HasActivatable` - `onActivate()` and `onDeactivate()` methods
     *
     * @tparam THandle The concrete `EntityHandle<TStrongId>` specialisation.
     *                 Used to scope `ComponentTypeId` and `ComponentOpsRegistry`.
     * @tparam TEntityManager The concrete `EntityManager` specialisation.
     *                        Used by the clone function to emplace components
     *                        on the target entity.
     *
     * @see ComponentOps
     * @see ComponentOpsRegistry
     * @see Traits
     */
    template<typename TEntityManager>
    class ComponentReflector {


    public:

        using Handle_type = TEntityManager::Handle_type;

        /**
         * @brief Registers a component type with the reflection system.
         *
         * @details Generates `ComponentOps` function pointers based on which
         * lifecycle hooks the type implements. Uses `if constexpr` for
         * zero-overhead when hooks are not present.
         *
         * @tparam T The component type to register. Must be copy-constructible
         *           for cloning support.
         */
        template<typename T>
        static void registerType() {

           ComponentOps ops{

                .onAcquire = [](void* ptr) {
                    if constexpr (HasOnAcquire<T>) {
                        static_cast<T*>(ptr)->onAcquire();
                    }
                },

                .onRelease = [](void* ptr) {
                    if constexpr (HasOnRelease<T>) {
                        static_cast<T*>(ptr)->onRelease();
                    }
                },

                .onRemove = [](void* ptr) -> bool {
                    if constexpr (HasOnRemove<T>) {
                        return static_cast<T*>(ptr)->onRemove();
                    }
                    return true;
                },

                .enable = [](void* ptr) {
                    if constexpr (HasToggleable<T>) {
                        static_cast<T*>(ptr)->enable();
                    }
                },

                .disable = [](void* ptr) {
                    if constexpr (HasToggleable<T>) {
                        static_cast<T*>(ptr)->disable();
                    }
                },

                .clone = [](void* managerRaw, const void* sourceRaw, const void* targetRaw) -> void* {

                    auto* manager = static_cast<TEntityManager*>(managerRaw);
                    const auto* source = static_cast<const T*>(sourceRaw);
                    const auto* target = static_cast<const Handle_type*>(targetRaw);

                    T* cmp = nullptr;
                    if constexpr (std::is_copy_constructible_v<T>) {
                        cmp = manager->template emplace<T>(*target, *source);
                    } else {
                        return nullptr;
                    }

                    if (!cmp) {
                        return nullptr;
                    }

                    if constexpr (HasClone<T>) {
                        cmp->onClone(*source);
                    }

                    return cmp;
                },

               .onActivate = [](void* ptr) {
                   if constexpr (HasActivatable<T>) {
                       static_cast<T*>(ptr)->onActivate();
                   }
                },

               .onDeactivate = [](void* ptr) {
                   if constexpr (HasActivatable<T>) {
                       static_cast<T*>(ptr)->onDeactivate();
                   }
                },

            };

            const auto typeId = ComponentTypeId<Handle_type>::template id<T>();

           ComponentOpsRegistry<Handle_type>::setOps(typeId, ops);

        }

    };


}