/**
 * @file TargetedCommand.ixx
 * @brief Abstract base class for commands targeting a specific GameObject.
 */
module;

export module helios.engine.core.messaging.command.TargetedCommand;

import helios.engine.game.GameObject;

export namespace helios::engine::core::messaging::command {

    class TargetedCommandDispatcher;

    /**
     * @brief Abstract base class for commands targeting a specific GameObject.
     *
     * @details A TargetedCommand encapsulates a single game action (e.g., movement, attack)
     * to be performed on a specific GameObject identified by its Guid.
     *
     * Concrete commands are typically created by input systems and queued in a
     * CommandBuffer. The buffer then executes them against the target GameObject
     * during the command flush phase.
     *
     * Commands support the Visitor pattern through accept(), enabling type-safe
     * dispatch to specialized handlers without explicit type checking.
     *
     * Example implementation:
     * ```cpp
     * class MoveCommand : public TargetedCommand {
     *     helios::math::vec3f direction_;
     *     float speed_;
     * public:
     *     MoveCommand(helios::math::vec3f dir, float speed)
     *         : direction_(dir), speed_(speed) {}
     *
     *     void execute(helios::engine::game::GameObject& obj) const noexcept override {
     *         auto* move = obj.get<Move2DComponent>();
     *         if (move) move->setVelocity(direction_ * speed_);
     *     }
     * };
     * ```
     *
     * @see WorldCommand for commands operating on the entire GameWorld
     * @see TargetedCommandDispatcher for type-safe command dispatch
     */
    class TargetedCommand {

    public:

        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~TargetedCommand() = default;

        /**
         * @brief Executes the command on the specified GameObject.
         *
         * @param gameObject The target GameObject to apply the command to.
         *
         * @note Implementations must be noexcept. If execute() could fail,
         *       handle errors internally (e.g., log and skip).
         */
        virtual void execute(helios::engine::game::GameObject& gameObject) const noexcept = 0;

        /**
         * @brief Accepts a dispatcher for type-safe command handling.
         *
         * @param gameObject The target GameObject context.
         * @param dispatcher The dispatcher to delegate command handling to.
         *
         * @details Override this method in derived commands to enable dispatch
         * to specialized handlers. The default implementation is a no-op.
         */
        virtual void accept(
            helios::engine::game::GameObject& gameObject,
            helios::engine::core::messaging::command::TargetedCommandDispatcher& dispatcher) const noexcept {
            // no-op by default
        }
    };


}