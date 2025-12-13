/**
 * @file Command.ixx
 * @brief Abstract base class for game commands following the Command pattern.
 */
module;

export module helios.engine.game.Command;

import helios.engine.game.GameObject;


export namespace helios::engine::game {

    /**
     * @brief Abstract base class for encapsulating game actions as executable commands.
     *
     * @details A `Command` encapsulates a single game action (e.g., movement, attack)
     * to be performed on a `GameObject`.
     *
     * Concrete commands are typically created by an `InputHandler` and queued in a
     * `CommandBuffer`. The buffer then executes them against the target `GameObject`
     * during a later processing step.
     *
     * Example implementation:
     * ```cpp
     * class MoveCommand : public helios::engine::game::Command {
     *     helios::math::vec3f direction_;
     *     float speed_;
     * public:
     *     MoveCommand(helios::math::vec3f dir, float speed)
     *         : direction_(dir), speed_(speed) {}
     *
     *     void execute(helios::engine::game::GameObject& obj) override {
     *         obj.translate(direction_ * speed_);
     *     }
     * };
     * ```
     *
     * @note Implementations should ensure execute() is exception-safe or noexcept,
     *       as throwing exceptions during CommandBuffer::flush() will abort execution
     *       of remaining commands.
     */
    class Command {

    public:

        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~Command() = default;

        /**
         * @brief Executes the command on the specified GameObject.
         *
         * @param gameObject The target GameObject to apply the command to.
         *
         * @note Derived classes must implement this method to define the specific
         *       action performed by the command.
         * @note This method should be exception-safe. If it throws, CommandBuffer::flush()
         *       will abort processing of remaining commands.
         */
        virtual void execute(helios::engine::game::GameObject& gameObject) = 0;
    };


}