/**
 * @file CommandHandlerRegistry.ixx
 * @brief Registry for mapping command types to their handlers.
 */
module;

#include <concepts>
#include <vector>
#include <cassert>

export module helios.engine.runtime.messaging.command.CommandHandlerRegistry;

import helios.engine.ecs.types.ComponentTypeId;
import helios.engine.runtime.messaging.command.types;

using namespace helios::engine::ecs::types;
using namespace helios::engine::runtime::messaging::command::types;


export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Type-erased storage entry for a registered command handler.
     */
    struct CommandHandlerEntry {
        /**
         * @brief Pointer to the owning object (the handler instance).
         */
        void* owner = nullptr;

        /**
         * @brief Type-erased static trampoline function that casts owner and command to concrete types.
         */
        bool (*submitFn)(void*, const void*) noexcept = nullptr;
    };

    /**
     * @brief Typed reference wrapper for invoking a registered handler.
     *
     * @tparam CommandType The specific command type this reference handles.
     */
    template<typename CommandType>
    struct CommandHandlerRef {

        /**
         * @brief Pointer to the handler instance.
         */
        void* owner = nullptr;

        /**
         * @brief Type-erased trampoline for command dispatch.
         */
        bool (*submitFn)(void*, const void*) noexcept = nullptr;

        /**
         * @brief Checks if this reference points to a valid handler.
         *
         * @return True if both owner and submitFn are non-null.
         */
        [[nodiscard]] explicit operator bool() const noexcept {
            return owner && submitFn;
        }

        /**
         * @brief Submits a command to the referenced handler.
         *
         * @param cmd The command instance to submit.
         * @return True if the command was accepted/handled.
         */
        bool submit(const CommandType& cmd) const noexcept {
            return submitFn(owner, &cmd);
        }

    };

    /**
     * @brief Registry that maps CommandType types to handler instances via function pointers.
     *
     * @details The CommandHandlerRegistry provides a mechanism to decouple command producers (systems)
     * from command consumers (managers). Handlers are registered by reference, and the registry
     * stores a type-erased "trampoline" function that allows invoking the handler's `submit(const Cmd&)`
     * method without knowing the concrete handler type at the call site.
     *
     * This avoids virtual inheritance (TypedCommandHandler) and allows any class with a matching
     * `submit` signature to act as a handler.
     *
     * Lookup is O(1) based on the CommandTypeId.
     */
    class CommandHandlerRegistry {

        /**
         * @brief Dense vector of handler entries, indexed by CommandTypeId value.
         */
        std::vector<CommandHandlerEntry> entries_;

    public:

        /**
         * @brief Registers an object as the handler for a specific command type.
         *
         * @details The handler object must provide a method:
         * `bool submit(const CommandType&) noexcept`.
         * Upon registration, a static lambda (trampoline) is generated to handle type
         * erasure and casting.
         *
         * @tparam CommandType The command type to handle.
         * @tparam OwningT The concrete type of the handler object.
         *
         * @param owner Reference to the handler instance. Must outlive the registry (usually Owned by GameWorld/ResourceRegistry).
         *
         * @pre No handler is currently registered for this command type.
         */
        template<typename CommandType, typename OwningT>
        void registerHandler(OwningT& owner) {
            static_assert(requires(OwningT& x, const CommandType& c) {
                { x.submit(c) } noexcept -> std::same_as<bool>;
            });

            const auto idx = CommandTypeId::id<CommandType>().value();

            if (entries_.size() <= idx) {
                entries_.resize(idx + 1);
            }

            assert(entries_[idx].owner == nullptr && "Handler already registered for this command type");

            entries_[idx] = CommandHandlerEntry{
                &owner,
                +[](void* owner, const void* cmd) noexcept -> bool {
                    return static_cast<OwningT*>(owner)->submit(*static_cast<const CommandType*>(cmd));
                }
            };
        }

        /**
         * @brief Checks if a handler is registered for the specified command type.
         *
         * @tparam CommandType The command type to check.
         *
         * @return True if a valid handler exists.
         */
        template<typename CommandType>
        [[nodiscard]] bool has() const noexcept {
            const auto idx = CommandTypeId::id<CommandType>().value();

            if (idx >= entries_.size()) {
                return false;
            }

            const auto& entry = entries_[idx];
            return entry.owner && entry.submitFn;
        }

        /**
         * @brief Retrieves a typed reference to the registered handler.
         *
         * @tparam CommandType The command type.
         *
         * @return A CommandHandlerRef wrapper. Can be checked for validity via operator bool().
         */
        template<typename CommandType>
        [[nodiscard]] CommandHandlerRef<CommandType> tryHandler() const noexcept {
            const auto idx = CommandTypeId::id<CommandType>().value();

            if (idx >= entries_.size()) {
                return {};
            }

            const auto& entry = entries_[idx];

            if (!entry.owner || !entry.submitFn) {
                return {};
            }

            return CommandHandlerRef<CommandType>{ entry.owner, entry.submitFn };
        }

        /**
         * @brief Directly submits a command to its registered handler.
         *
         * @tparam CommandType The command type.
         *
         * @param cmd The command instance.
         *
         * @return True if a handler was found and it returned true; false otherwise.
         */
        template<typename CommandType>
        bool submit(const CommandType& cmd) const noexcept {
            if (auto handler = tryHandler<CommandType>()) {
                return handler.submit(cmd);
            }
            return false;
        }

    };

} // namespace