/**
 * @file WorldCommandDispatcher.ixx
 * @brief Abstract dispatcher interface for WorldCommand handling.
 */
module;

export module helios.engine.core.messaging.command.WorldCommandDispatcher;

import helios.engine.game.GameWorld;


export namespace helios::engine::core::messaging::command {

    class WorldCommand;

    /**
     * @brief Abstract dispatcher interface for type-safe WorldCommand handling.
     *
     * @details WorldCommandDispatcher enables the Visitor pattern for WorldCommand
     * processing. When registered with a CommandBuffer, the dispatcher receives
     * commands of the appropriate type and can route them to specialized handlers
     * (e.g., managers, pools, or systems).
     *
     * This decouples command execution from command definition, allowing commands
     * to be processed by external systems without the command knowing about them.
     *
     * @see TypedWorldCommandDispatcher for type-safe derived implementation
     * @see CommandBuffer::addDispatcher for registration
     */
    class WorldCommandDispatcher {

    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived dispatchers.
         */
        virtual ~WorldCommandDispatcher() = default;

        /**
         * @brief Dispatches a WorldCommand for specialized handling.
         *
         * @param gameWorld The game world context.
         * @param command The command to dispatch.
         */
        virtual void dispatch(
            helios::engine::game::GameWorld& gameWorld,
            const WorldCommand& command
        ) = 0;
    };


}