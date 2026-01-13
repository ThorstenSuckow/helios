/**
 * @file TypedWorldCommandDispatcher.ixx
 * @brief Type-safe dispatcher template for WorldCommand handling.
 */
module;

#include <memory>

export module helios.engine.core.messaging.command.TypedWorldCommandDispatcher;

import helios.engine.core.messaging.command.WorldCommand;
import helios.engine.core.messaging.command.WorldCommandDispatcher;


export namespace helios::engine::game {
    class GameWorld;
}

export namespace helios::engine::core::messaging::command {

    /**
     * @brief Type-safe dispatcher template for handling specific WorldCommand types.
     *
     * @tparam T The concrete WorldCommand type this dispatcher handles.
     *
     * @details TypedWorldCommandDispatcher provides a type-safe bridge between the
     * polymorphic dispatch() method and a typed dispatchTyped() handler. Derived
     * classes override dispatchTyped() to handle commands with full type information.
     *
     * @see WorldCommandDispatcher for the base interface
     * @see CommandBuffer::addDispatcher for registration
     */
    template<typename T>
    class TypedWorldCommandDispatcher : public WorldCommandDispatcher {
        static_assert(std::is_base_of_v<WorldCommand, T>, "T must derive from WorldCommand");

    protected:

        /**
         * @brief Handles the dispatched command with full type information.
         *
         * @param gameWorld The game world context.
         * @param command The typed command to process.
         *
         * @note Implementations must be noexcept.
         */
        virtual void dispatchTyped(
            helios::engine::ecs::GameWorld& gameWorld,
            const T& command
        ) noexcept = 0;

    public:

        /**
         * @brief Dispatches the command by downcasting to the concrete type.
         *
         * @param gameWorld The game world context.
         * @param command The base command reference.
         */
        void dispatch(
            helios::engine::ecs::GameWorld& gameWorld,
            const WorldCommand& command
        ) override {
            dispatchTyped(gameWorld, static_cast<const T&>(command));
        }

    };


}