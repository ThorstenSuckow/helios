/**
 * @file TimerCommandDispatcher.ixx
 * @brief Dispatcher for routing timer control commands to the appropriate handler.
 */
module;

#include <cassert>

export module helios.engine.mechanics.timing.TimerCommandDispatcher;

import helios.engine.mechanics.timing.commands;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::mechanics::timing {

    using namespace helios::engine::mechanics::timing::commands;
    using namespace helios::engine::runtime::world;
    using namespace helios::engine::runtime::messaging::command;

    /**
     * @brief Dispatches TimerControlCommands to the registered TimerCommandHandler.
     *
     * Routes incoming timer commands to the game world's timer command handler
     * via the Visitor pattern.
     *
     * @see TimerCommandHandler
     * @see TimerControlCommand
     */
    class TimerCommandDispatcher final : public TypedWorldCommandDispatcher<TimerControlCommand> {

    protected:

        /**
         * @brief Dispatches the command to the game world's timer command handler.
         *
         * @param gameWorld Reference to the game world.
         * @param command The TimerControlCommand to dispatch.
         */
        void dispatchTyped(GameWorld& gameWorld, const TimerControlCommand& command) noexcept override {

            if (auto* timerCommandHandler = gameWorld.timerCommandHandler()) {
                timerCommandHandler->submit(command);
            }
        };


    public:

        TimerCommandDispatcher() = default;

    };


}