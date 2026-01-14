/**
 * @file TargetedCommandDispatcher.ixx
 * @brief Abstract dispatcher interface for TargetedCommand handling.
 */
module;

export module helios.engine.runtime.messaging.command.TargetedCommandDispatcher;

import helios.engine.ecs.GameObject;


export namespace helios::engine::runtime::messaging::command {

    class TargetedCommand;

    /**
     * @brief Abstract dispatcher interface for type-safe TargetedCommand handling.
     *
     * @details TargetedCommandDispatcher enables the Visitor pattern for TargetedCommand
     * processing. When registered with a CommandBuffer, the dispatcher receives
     * commands of the appropriate type and can route them to specialized handlers
     * (e.g., component systems, managers).
     *
     * This decouples command execution from command definition, allowing commands
     * to be processed by external systems without the command knowing about them.
     *
     * @see TypedTargetedCommandDispatcher for type-safe derived implementation
     * @see CommandBuffer::addDispatcher for registration
     */
    class TargetedCommandDispatcher {

    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived dispatchers.
         */
        virtual ~TargetedCommandDispatcher() = default;

        /**
         * @brief Dispatches a TargetedCommand for specialized handling.
         *
         * @param gameObject The target GameObject.
         * @param command The command to dispatch.
         */
        virtual void dispatch(
            helios::engine::ecs::GameObject& gameObject,
            const TargetedCommand& command
        ) = 0;
    };


}