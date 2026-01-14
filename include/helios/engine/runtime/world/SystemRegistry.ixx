/**
 * @file SystemRegistry.ixx
 * @brief Registry for managing System instances within a game loop phase.
 */
module;

#include <memory>
#include <vector>
#include <cassert>


export module helios.engine.runtime.world.SystemRegistry;

import helios.engine.ecs.System;


export namespace helios::engine::runtime::world {

    /**
     * @brief Container for System instances within a game loop pass.
     *
     * @details SystemRegistry manages a collection of Systems that execute
     * together within a single pass of the game loop. It provides type-safe
     * registration, lookup, and iteration over Systems.
     *
     * ## Ownership
     *
     * The registry owns all registered Systems via `std::unique_ptr`. Systems
     * are destroyed when the registry is destroyed.
     *
     * ## Type-Safe Access
     *
     * Systems can be retrieved by type using `getSystem<T>()`, enabling
     * inter-system communication when necessary.
     *
     * Example:
     * ```cpp
     * SystemRegistry registry;
     *
     * // Add systems
     * registry.add<Move2DSystem>(gameWorld);
     * registry.add<CollisionSystem>(gameWorld);
     *
     * // Check for system
     * if (registry.hasSystem<CollisionSystem>()) {
     *     auto* collision = registry.getSystem<CollisionSystem>();
     * }
     *
     * // Iterate all systems
     * for (auto& system : registry.systems()) {
     *     system->update(ctx);
     * }
     * ```
     *
     * @see System
     * @see GameLoop
     * @see GameLoopPhase
     */
    class SystemRegistry {

        /**
         * @brief Collection of owned System instances.
         */
        std::vector<std::unique_ptr<helios::engine::ecs::System>> systems_;

    public:

        /**
         * @brief Returns a reference to the system collection.
         *
         * @return Reference to the vector of System unique_ptrs.
         */
        [[nodiscard]] std::vector<std::unique_ptr<helios::engine::ecs::System>>& systems() noexcept {
            return systems_;
        }

        /**
         * @brief Adds a new System of type T to the registry.
         *
         * @tparam T The System type to add. Must derive from System.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the System constructor.
         *
         * @return Reference to the newly added System.
         *
         * @pre No System of type T is already registered.
         */
        template<typename T, typename... Args>
        requires std::is_base_of_v<helios::engine::ecs::System, T>
        T& add(Args&&... args) {
            assert(!hasSystem<T>() && "System already registered with GameLoopPhase");
            auto system_ptr = std::make_unique<T>(std::forward<Args>(args)...);
            T* raw_ptr = system_ptr.get();
            systems_.push_back(std::move(system_ptr));
            return *raw_ptr;
        }

        /**
         * @brief Checks if a System of type T is registered.
         *
         * @tparam T The System type to check for.
         *
         * @return True if the System exists, false otherwise.
         */
        template<typename T>
        requires std::is_base_of_v<helios::engine::ecs::System, T>
        [[nodiscard]] bool hasSystem() const {
            return getSystem<T>() != nullptr;
        }

        /**
         * @brief Retrieves a System by type.
         *
         * @tparam T The System type to retrieve.
         *
         * @return Pointer to the System if found, nullptr otherwise.
         *
         * @note Uses dynamic_cast for type checking. O(n) complexity.
         */
        template<typename T>
        requires std::is_base_of_v<helios::engine::ecs::System, T>
        [[nodiscard]] T* getSystem() const {
            for (const auto& system : systems_) {
                if (auto* sys = dynamic_cast<T*>(system.get())) {
                    return sys;
                }
            }

            return nullptr;
        }

    };


}