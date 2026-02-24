/**
 * @file WorldLifecycleCommand.ixx
 * @brief Command for requesting world-level lifecycle operations.
 */
module;

#include <cstdint>

export module helios.engine.runtime.world.commands.WorldLifecycleCommand;

export namespace helios::engine::runtime::world::commands {

    /**
     * @brief Actions that can be requested via WorldLifecycleCommand.
     */
    enum class WorldLifecycleAction : uint8_t {
        /**
         * @brief Resets all managers and the session to their initial state.
         */
        Reset = 0
    };

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

