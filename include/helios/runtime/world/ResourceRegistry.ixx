/**
 * @file ResourceRegistry.ixx
 * @brief Type-indexed registry for engine resources with O(1) lookup.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>

export module helios.runtime.world.ResourceRegistry;

import helios.runtime.world.Manager;

import helios.core.memory.ErasedUnique;

import helios.runtime.messaging.command.CommandBuffer;
import helios.runtime.messaging.command.CommandBufferRegistry;
import helios.runtime.messaging.command.CommandHandlerRegistry;

import helios.runtime.world.ManagerRegistry;

import helios.runtime.concepts;
import helios.runtime.messaging.command.concepts;
import helios.runtime.world.types;

using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;
using namespace helios::runtime::world::types;
using namespace helios::core::memory;
using namespace helios::runtime::world::concepts;
using namespace helios::runtime::messaging::command::concepts;
export namespace helios::runtime::world {


    /**
     * @brief Unified type-indexed registry for all engine resources.
     *
     * @details ResourceRegistry acts as the central access point for Managers
     * and CommandBuffers. It delegates to a ManagerRegistry and a
     * CommandBufferRegistry internally, routing registration and lookup
     * based on compile-time concept checks (IsManagerLike / IsCommandBufferLike).
     *
     * @see ManagerRegistry
     * @see CommandBufferRegistry
     * @see GameWorld
     */
   class ResourceRegistry {

        /**
         * @brief Registry for Manager instances.
         */
        ManagerRegistry managerRegistry_;

        /**
         * @brief Registry for CommandBuffer instances.
         */
        CommandBufferRegistry commandBufferRegistry_;

        public:

        /**
         * @brief Registers and constructs a resource of type T.
         *
         * @details Routes to ManagerRegistry or CommandBufferRegistry based
         * on whether T satisfies IsManagerLike or IsCommandBufferLike. The resource
         * is constructed in-place with forwarded arguments.
         *
         * @tparam T The resource type. Must satisfy IsManagerLike or IsCommandBufferLike.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @return Reference to the newly registered resource.
         */
        template<class T, class... Args>
        requires IsManagerLike<T> || IsCommandBufferLike<T>
        T& emplace(Args&&... args) {

            if constexpr (IsManagerLike<T>) {
                return managerRegistry_.template add<T>(std::forward<Args>(args)...);
            }

            if constexpr (IsCommandBufferLike<T>) {
                return  commandBufferRegistry_.template add<T>(std::forward<Args>(args)...);
            }

            assert(false);
        }

        /**
         * @brief Checks if a resource of type T is registered.
         *
         * @tparam T The resource type.
         *
         * @return True if the resource is registered.
         */
        template<class T>
        bool has() const noexcept {
            if constexpr (IsManagerLike<T>) {
                return managerRegistry_.has<T>();
            }

            if constexpr (IsCommandBufferLike<T>) {
                return commandBufferRegistry_.has<T>();
            }

            return false;
        }

        /**
         * @brief Retrieves a registered resource by type.
         *
         * @tparam T The resource type.
         *
         * @return Pointer to the resource, or nullptr if not registered.
         */
        template<class T>
        T* tryGet() const noexcept {
            if constexpr (IsManagerLike<T>) {
                return managerRegistry_.item<T>();
            }

            if constexpr (IsCommandBufferLike<T>) {
                return commandBufferRegistry_.item<T>();
            }

            return nullptr;
        }


        /**
         * @brief Retrieves a registered resource by type (checked).
         *
         * @details Asserts that the resource is registered before returning.
         *
         * @tparam T The resource type.
         *
         * @return Reference to the resource.
         *
         * @pre The resource must be registered.
         */
        template<class T>
        T& get() const noexcept {
            assert(has<T>() && "Resource not found");
            if constexpr (IsManagerLike<T>) {
                return *managerRegistry_.item<T>();
            }

            if constexpr (IsCommandBufferLike<T>) {
                return *commandBufferRegistry_.item<T>();
            }

            assert(false);
        }

        /**
         * @brief Returns a read-only span of all registered Managers.
         *
         * @return Span of Manager pointers in registration order.
         */
        [[nodiscard]] std::span<Manager* const> managers() const noexcept {
            return managerRegistry_.items();
        }

        /**
         * @copydoc managers() const
         */
        std::span<Manager*> managers() noexcept {
            return managerRegistry_.items();
        }

        /**
         * @brief Returns a read-only span of all registered CommandBuffers.
         *
         * @return Span of CommandBuffer pointers in registration order.
         */
        std::span<CommandBuffer* const> commandBuffers() const noexcept {
            return commandBufferRegistry_.items();
        }

        /**
         * @copydoc commandBuffers() const
         */
        std::span<CommandBuffer*> commandBuffers() noexcept {
            return commandBufferRegistry_.items();
        }
    };
}

