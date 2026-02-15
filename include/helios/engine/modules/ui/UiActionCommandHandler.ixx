/**
 * @file UiActionCommandHandler.ixx
 * @brief Interface for UI action command handlers.
 */
module;

export module helios.engine.modules.ui.UiActionCommandHandler;

import helios.engine.modules.ui.commands.UiActionCommand;

export namespace helios::engine::modules::ui {

    /**
     * @brief Abstract interface for handling UI action commands.
     *
     * Implementations receive UiActionCommand instances and process them
     * to execute UI actions (e.g., button clicks, menu selections).
     *
     * @see UiActionCommandManager
     * @see UiActionCommand
     */
    class UiActionCommandHandler {

    public:

        virtual ~UiActionCommandHandler() = default;

        /**
         * @brief Submits a UI action command for processing.
         *
         * @param uiActionCommand The command to submit.
         * @return True if the command was accepted.
         */
        virtual bool submit(
            const helios::engine::modules::ui::commands::UiActionCommand uiActionCommand
        ) noexcept = 0;

    };

}