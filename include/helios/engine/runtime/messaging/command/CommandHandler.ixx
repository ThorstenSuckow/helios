/**
 * @file CommandHandler.ixx
 * @brief Abstract base class for command handlers.
 */
module;


export module helios.engine.runtime.messaging.command.CommandHandler;


export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Abstract base for type-erased command handler storage.
     *
     * @details CommandHandler is a non-templated base class that enables the
     * ResourceRegistry to store heterogeneous TypedCommandHandler instances
     * in a uniform collection. It carries no behavior itself; all dispatch
     * logic resides in the derived TypedCommandHandler.
     *
     * @see TypedCommandHandler
     * @see ResourceRegistry
     */
    class CommandHandler {

    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~CommandHandler() = default;

    };


}