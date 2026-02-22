/**
 * @file TimerCommandHandler.ixx
 * @brief Abstract handler interface for timer control commands.
 */
module;

export module helios.engine.mechanics.timing.TimerCommandHandler;

import helios.engine.mechanics.timing.commands;


export namespace helios::engine::mechanics::timing {

    using namespace helios::engine::mechanics::timing::commands;

    /**
     * @brief Abstract interface for handling TimerControlCommands.
     *
     * Implementations receive timer control commands and translate them
     * into concrete timer state transitions. TimerManager is the primary
     * implementation.
     *
     * @see TimerManager
     * @see TimerCommandDispatcher
     */
    class TimerCommandHandler {

    public:

        /**
         * @brief Virtual destructor.
         */
        virtual ~TimerCommandHandler() = default;

        /**
         * @brief Submits a timer control command for processing.
         *
         * @param timerControlCommand The command to process.
         *
         * @return True if the command was accepted.
         */
        virtual bool submit(
            const TimerControlCommand& timerControlCommand
        ) noexcept = 0;


    };

}