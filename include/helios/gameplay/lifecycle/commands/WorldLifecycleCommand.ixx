/**
 * @file WorldLifecycleCommand.ixx
 * @brief Command for requesting world-level lifecycle operations.
 */
module;

export module helios.gameplay.lifecycle.commands.WorldLifecycleCommand;


import helios.gameplay.lifecycle.types;

using namespace helios::gameplay::lifecycle::types;

export namespace helios::gameplay::lifecycle::commands {

    /**
     * @brief Command for requesting a world-level lifecycle operation.
     *
     * @details Encapsulates a WorldLifecycleAction that is routed through
     * the EngineCommandBuffer to the WorldLifecycleManager. The manager
     * executes the requested action during its flush cycle.
     *
     * @see WorldLifecycleAction
     * @see WorldLifecycleManager
     */
    class WorldLifecycleCommand {

        /**
         * @brief The lifecycle action to perform.
         */
        WorldLifecycleAction action_;

    public:

        /**
         * @brief Constructs a command with the specified action.
         *
         * @param action The lifecycle action to request.
         */
        explicit WorldLifecycleCommand(const WorldLifecycleAction action) noexcept
            : action_(action) {}

        /**
         * @brief Returns the requested lifecycle action.
         *
         * @return The lifecycle action.
         */
        [[nodiscard]] WorldLifecycleAction action() const noexcept {
            return action_;
        }
    };

}

