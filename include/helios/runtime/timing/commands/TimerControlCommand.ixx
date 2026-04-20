/**
 * @file TimerControlCommand.ixx
 * @brief Command for controlling game timer state transitions.
 */
module;

#include <cassert>
#include <memory>

export module helios.runtime.timing.commands.TimerControlCommand;


import helios.runtime.timing.types;



export namespace helios::runtime::timing::commands {

    /**
     * @brief Command that carries a TimerControlContext to the timer subsystem.
     *
     * TimerControlCommand is dispatched through the command buffer to request
     * timer state transitions (start, pause, stop).
     *
     * @see helios::runtime::timing::types::TimerControlContext
     * @see helios::runtime::timing::TimerCommandDispatcher
     */
    class TimerControlCommand  {

        /**
         * @brief The timer control context describing the requested state transition.
         */
        helios::runtime::timing::types::TimerControlContext timerContext_;

    public:

        /**
         * @brief Constructs a TimerControlCommand with the given context.
         *
         * @param timerContext The context describing which timer to control and the target state.
         */
        explicit TimerControlCommand(
            helios::runtime::timing::types::TimerControlContext timerContext
        ) : timerContext_(std::move(timerContext)) { }


        /**
         * @brief Returns the timer control context.
         *
         * @return The TimerControlContext describing the requested state transition.
         */
        [[nodiscard]] helios::runtime::timing::types::TimerControlContext timerControlContext() const noexcept {
            return timerContext_;
        }


    };


}