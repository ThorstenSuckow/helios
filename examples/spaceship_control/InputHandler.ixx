/**
 * @file InputHandler.ixx
 * @brief Input handler implementation for the spaceship control example.
 */
module;

#include <memory>

export module helios.examples.spaceshipControl.InputHandler;

import helios.engine.game.InputHandler;
import helios.input.gamepad.GamepadState;
import helios.engine.game.Command;
import helios.engine.game.InputSnapshot;
import helios.engine.game.CommandBuffer;
import helios.engine.game.GameObject;
import helios.examples.spaceshipControl.commands.PlayerMoveCommand;
import helios.util.Guid;

export namespace helios::examples::spaceshipControl {

    /**
     * @brief Translates gamepad input into spaceship movement commands.
     *
     * @details This InputHandler reads the left analog stick from the captured InputSnapshot
     * and generates a PlayerMoveCommand for the target GameObject. The stick's direction
     * is normalized and passed along with the magnitude (speed factor) to enable
     * analog-sensitive movement control.
     *
     * @see helios::engine::game::InputHandler
     * @see PlayerMoveCommand
     */
    class InputHandler : public helios::engine::game::InputHandler {

    public:

        /**
         * @brief Processes input and queues movement commands for the spaceship.
         *
         * @param inputSnapshot The captured input state containing gamepad data.
         * @param guid The unique identifier of the target GameObject.
         * @param commandBuffer The buffer to queue generated commands into.
         *
         * @note Always generates a PlayerMoveCommand, even when the stick is neutral.
         *       The command itself handles deadzone filtering.
         */
        void handleInput(
            const helios::engine::game::InputSnapshot& inputSnapshot,
            const helios::util::Guid& guid,
            helios::engine::game::CommandBuffer& commandBuffer
        ) override {

            auto stick = inputSnapshot.gamepadState().left();
            float speed      = stick.length();
            speed = speed == 0.0f ? 0.0f : speed;

            helios::math::vec2f dir = speed > 0.0f ? stick * (1.0f/speed) : helios::math::vec2f{0.0f, 0.0f};

            commandBuffer.add(
                guid,
                std::make_unique<
                helios::examples::spaceshipControl::commands::PlayerMoveCommand
                >(dir, speed)
            );


        }


    };



}