/**
 * @file TimerControlCommand.ixx
 * @brief Command for controlling game timer state transitions.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.mechanics.timing.commands.TimerControlCommand;


import helios.engine.mechanics.timing.types;



export namespace helios::engine::mechanics::timing::commands {

    /**
     * @brief Command that carries a TimerControlContext to the timer subsystem.
     *
     * TimerControlCommand is dispatched through the command buffer to request
     * timer state transitions (start, pause, stop).
     *
     * @see helios::engine::mechanics::timing::types::TimerControlContext
     * @see helios::engine::mechanics::timing::TimerCommandDispatcher
     */
    class TimerControlCommand  {

        /**
         * @brief The timer control context describing the requested state transition.
         */
        helios::engine::mechanics::timing::types::TimerControlContext timerContext_;

    public:

        /**
         * @brief Constructs a TimerControlCommand with the given context.
         *
         * @param timerContext The context describing which timer to control and the target state.
         */
        explicit TimerControlCommand(
            helios::engine::mechanics::timing::types::TimerControlContext timerContext
        ) : timerContext_(std::move(timerContext)) { }


        /**
         * @brief Returns the timer control context.
         *
         * @return The TimerControlContext describing the requested state transition.
         */
        [[nodiscard]] helios::engine::mechanics::timing::types::TimerControlContext timerControlContext() const noexcept {
            return timerContext_;
        }


    };


}