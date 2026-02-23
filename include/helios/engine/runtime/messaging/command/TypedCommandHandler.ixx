/**
 * @file TypedCommandHandler.ixx
 * @brief Type-safe command handler for deferred command routing.
 */
module;


export module helios.engine.runtime.messaging.command.TypedCommandHandler;

import helios.engine.runtime.messaging.command.CommandHandler;

export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Type-safe handler for a specific command type.
     *
     * @details TypedCommandHandler provides the interface for receiving commands
     * of a specific type during the TypedCommandBuffer flush. Managers that
     * process commands implement this for each command type they handle.
     *
     * A single Manager may implement multiple TypedCommandHandler specializations:
     *
     * ```cpp
     * class SpawnManager : public Manager,
     *                      public TypedCommandHandler<SpawnCommand>,
     *                      public TypedCommandHandler<DespawnCommand> {
     *     bool submit(SpawnCommand cmd) noexcept override { ... }
     *     bool submit(DespawnCommand cmd) noexcept override { ... }
     * };
     * ```
     *
     * Handlers are registered with the ResourceRegistry during Manager::init()
     * and looked up by the TypedCommandBuffer at flush time.
     *
     * @tparam CommandType The command type this handler accepts.
     *
     * @see CommandHandler
     * @see TypedCommandBuffer
     * @see Manager
     */
    template<typename CommandType>
    class TypedCommandHandler : public CommandHandler{

    public:

        /**
         * @brief Submits a command for deferred processing.
         *
         * @details Called by the TypedCommandBuffer during flush when a handler
         * is registered for this command type. The handler typically enqueues
         * the command for batch processing during Manager::flush().
         *
         * @param cmd The command to process.
         *
         * @return True if the command was accepted.
         */
        virtual bool submit(CommandType cmd) noexcept = 0;

    };


}