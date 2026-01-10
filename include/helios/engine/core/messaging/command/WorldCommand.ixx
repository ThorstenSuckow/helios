/**
 * @file WorldCommand.ixx
 * @brief Abstract base class for commands operating on the entire GameWorld.
 */
module;

export module helios.engine.core.messaging.command.WorldCommand;

import helios.engine.game.GameWorld;

export namespace helios::engine::core::messaging::command {

    class WorldCommandDispatcher;

    /**
     * @brief Abstract base class for commands that operate on the entire GameWorld.
     *
     * @details Unlike TargetedCommand which operates on a specific GameObject, a
     * WorldCommand executes against the entire GameWorld. This is useful for global
     * game actions such as spawning entities, clearing levels, or applying world-wide
     * effects.
     *
     * WorldCommands support the Visitor pattern through accept(), enabling type-safe
     * dispatch to specialized handlers without explicit type checking.
     *
     * @see TargetedCommand for commands targeting specific GameObjects
     * @see WorldCommandDispatcher for type-safe command dispatch
     */
    class WorldCommand {

    public:

        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~WorldCommand() = default;

        /**
         * @brief Executes the command against the entire GameWorld.
         *
         * @param gameWorld The game world to apply the command to.
         *
         * @note Implementations must be exception-safe (noexcept).
         */
        virtual void execute(helios::engine::game::GameWorld& gameWorld) const noexcept = 0;

        /**
         * @brief Accepts a dispatcher for type-safe command handling.
         *
         * @param gameWorld The game world context.
         * @param dispatcher The dispatcher to delegate command handling to.
         *
         * @details Override this method in derived commands to enable dispatch
         * to specialized handlers. The default implementation is a no-op.
         */
        virtual void accept(
            helios::engine::game::GameWorld& gameWorld,
            helios::engine::core::messaging::command::WorldCommandDispatcher& dispatcher) const noexcept {
            // no-op by default
        }
    };


}