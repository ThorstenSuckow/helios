/**
 * @file InputHandler.ixx
 * @brief Abstract interface for translating input snapshots into game commands.
 */
module;

export module helios.engine.game.InputHandler;

import helios.engine.game.Command;
import helios.engine.game.CommandBuffer;
import helios.engine.game.InputSnapshot;
import helios.engine.game.GameObject;
import helios.util.Guid;

export namespace helios::engine::game {

    /**
     * @brief Abstract interface for processing input and generating game commands.
     *
     * @details The `InputHandler` translates raw input data, captured in an `InputSnapshot`,
     * into executable `Commands`. Handlers receive an immutable input snapshot and generate
     * commands that are queued in a `CommandBuffer` for later execution.
     *
     * Example implementation:
     * ```cpp
     * class PlayerInputHandler : public helios::engine::game::InputHandler {
     * public:
     *     void handleInput(
     *         const InputSnapshot& snapshot,
     *         const GameObject& target,
     *         CommandBuffer& buffer,
     *         float deltaTime
     *     ) override {
     *         auto& pad = snapshot.gamepadState();
     *
     *         // Movement
     *         if (pad.left().length() > 0.1f) {
     *             helios::math::vec3f dir(pad.left()[0], 0.0f, -pad.left()[1]);
     *             buffer.add(target, std::make_unique<MoveCommand>(dir, deltaTime * 5.0f));
     *         }
     *
     *         // Jump
     *         if (pad.buttonA()) {
     *             buffer.add(target, std::make_unique<JumpCommand>());
     *         }
     *     }
     * };
     * ```
     */
    class InputHandler {

    public:

        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~InputHandler() = default;

        /**
         * @brief Processes input snapshot and generates commands for a GameObject identified by Guid.
         *
         * @param inputSnapshot Immutable snapshot of input state at a specific point in time.
         * @param guid The unique identifier of the target GameObject.
         * @param commandBuffer Buffer to queue generated commands into.
         * @param deltaTime Time elapsed since last frame, in seconds.
         *
         * @note This overload is useful for scenarios like network synchronization or command replays
         *       where the target `GameObject` is identified by its `Guid` instead of a direct reference.
         * @note The target GameObject does not need to exist at the time handleInput() is called.
         *       Command execution will be skipped if the Guid is not found during flush().
         */
        virtual void handleInput(
            const helios::engine::game::InputSnapshot& inputSnapshot,
            const helios::util::Guid& guid,
            helios::engine::game::CommandBuffer& commandBuffer
        ) = 0;


    };


}