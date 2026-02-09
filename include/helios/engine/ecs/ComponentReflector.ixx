/**
 * @file ComponentReflector.ixx
 * @brief Compile-time reflection for component lifecycle hook registration.
 */
module;

#include <type_traits>

export module helios.engine.ecs.ComponentReflector;

import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.ComponentOps;
import helios.engine.ecs.Traits;
import helios.engine.core.data.ComponentTypeId;
import helios.engine.ecs.ComponentOpsRegistry;
import helios.engine.ecs.EntityManager;

export namespace helios::engine::ecs {

    /**
     * @brief Generates and registers ComponentOps for a component type.
     *
     * @details `ComponentReflector` uses compile-time trait detection to
     * generate appropriate function pointers for each lifecycle hook that
     * a component type implements. This enables zero-overhead conditional
     * invocation at runtime.
     *
     * ## Usage
     *
     * ```cpp
     * // Register a component type
     * ComponentReflector::registerType<HealthComponent>();
     *
     * // Or use the module registry pattern
     * inline void registerComponents() {
     *     using R = ComponentReflector;
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
     * @see ComponentOps
     * @see ComponentOpsRegistry
     * @see Traits
     */
    class ComponentReflector {

    public:

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
                    if constexpr (helios::engine::ecs::traits::HasOnAcquire<T>) {
                        static_cast<T*>(ptr)->onAcquire();
                    }
                },

                .onRelease = [](void* ptr) {
                    if constexpr (helios::engine::ecs::traits::HasOnRelease<T>) {
                        static_cast<T*>(ptr)->onRelease();
                    }
                },

                .onRemove = [](void* ptr) -> bool {
                    if constexpr (helios::engine::ecs::traits::HasOnRemove<T>) {
                        return static_cast<T*>(ptr)->onRemove();
                    }
                    return true;
                },

                .enable = [](void* ptr) {
                    if constexpr (helios::engine::ecs::traits::HasToggleable<T>) {
                        static_cast<T*>(ptr)->enable();
                    }
                },

                .disable = [](void* ptr) {
                    if constexpr (helios::engine::ecs::traits::HasToggleable<T>) {
                        static_cast<T*>(ptr)->disable();
                    }
                },

                .clone = [](void* managerRaw, const void* sourceRaw, const EntityHandle* target) -> void* {

                    auto* manager = static_cast<helios::engine::ecs::EntityManager*>(managerRaw);
                    const auto* source = static_cast<const T*>(sourceRaw);

                    T* cmp = nullptr;
                    if constexpr (std::is_copy_constructible_v<T>) {
                        cmp = manager->template emplace<T>(*target, *source);
                    } else {
                        return nullptr;
                    }

                    if (!cmp) {
                        return nullptr;
                    }

                    if constexpr (helios::engine::ecs::traits::HasClone<T>) {
                        cmp->onClone(*source);
                    }

                    return cmp;
                },

               .onActivate = [](void* ptr) {
                   if constexpr (helios::engine::ecs::traits::HasActivatable<T>) {
                       static_cast<T*>(ptr)->onActivate();
                   }
                },

               .onDeactivate = [](void* ptr) {
                   if constexpr (helios::engine::ecs::traits::HasActivatable<T>) {
                       static_cast<T*>(ptr)->onDeactivate();
                   }
                },

            };

            const auto typeId = helios::engine::core::data::ComponentTypeId::id<T>();

            ComponentOpsRegistry::setOps(typeId, ops);

        }

    };


}