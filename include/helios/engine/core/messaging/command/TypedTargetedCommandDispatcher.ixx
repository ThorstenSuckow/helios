/**
 * @file TypedTargetedCommandDispatcher.ixx
 * @brief Type-safe dispatcher template for TargetedCommand handling.
 */
module;

#include <memory>

export module helios.engine.core.messaging.command.TypedTargetedCommandDispatcher;

import helios.engine.core.messaging.command.TargetedCommandDispatcher;

import helios.engine.game.GameObject;


export namespace helios::engine::core::messaging::command {

    class TargetedCommand;

    /**
     * @brief Type-safe dispatcher template for handling specific TargetedCommand types.
     *
     * @tparam T The concrete TargetedCommand type this dispatcher handles.
     *
     * @details TypedTargetedCommandDispatcher provides a type-safe bridge between the
     * polymorphic dispatch() method and a typed dispatchTyped() handler. Derived
     * classes override dispatchTyped() to handle commands with full type information.
     *
     * @see TargetedCommandDispatcher for the base interface
     * @see CommandBuffer::addDispatcher for registration
     */
    template<typename T>
    class TypedTargetedCommandDispatcher : public TargetedCommandDispatcher {
        static_assert(std::is_base_of_v<TargetedCommand, T>, "T must derive from TargetedCommand");

    protected:

        /**
         * @brief Handles the dispatched command with full type information.
         *
         * @param gameObject The target GameObject.
         * @param command The typed command to process.
         *
         * @note Implementations must be noexcept.
         */
        virtual void dispatchTyped(
            helios::engine::game::GameObject& gameObject,
            const T& command
        ) noexcept = 0;

    public:

        /**
         * @brief Dispatches the command by downcasting to the concrete type.
         *
         * @param gameObject The target GameObject.
         * @param command The base command reference.
         */
        void dispatch(
            helios::engine::game::GameObject& gameObject,
            const TargetedCommand& command
        ) override {
            dispatchTyped(gameObject, static_cast<const T&>(command));
        }
    };


}