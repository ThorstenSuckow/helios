/**
 * @file InputHandler.ixx
 * @brief Input handler implementation for the spaceship control example.
 */
module;

#include <memory>

export module helios.examples.spaceshipShooting.InputHandler;

import helios.engine.game.InputHandler;
import helios.input.gamepad.GamepadState;
import helios.engine.game.Command;
import helios.engine.game.InputSnapshot;
import helios.engine.game.CommandBuffer;
import helios.engine.game.GameObject;
import helios.examples.spaceshipShooting.commands.PlayerMoveCommand;
import helios.examples.spaceshipShooting.commands.PlayerShootCommand;
import helios.util.Guid;
import helios.math.utils;

export namespace helios::examples::spaceshipShooting {

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

            const auto leftStick = inputSnapshot.gamepadState().left();
            float speed  = leftStick.length();
            speed = speed <= helios::math::EPSILON_LENGTH ? 0.0f : speed;
            helios::math::vec2f ldir = speed == 0.0f ? helios::math::vec2f{0.0f, 0.0f} : leftStick.normalize();

            commandBuffer.add(
                guid,
                std::make_unique<
                helios::examples::spaceshipShooting::commands::PlayerMoveCommand
                >(ldir, speed)
            );

            const auto rightStick = inputSnapshot.gamepadState().right();
            float freq  = rightStick.length();
            freq = freq <= helios::math::EPSILON_LENGTH ? 0.0f : freq;
            helios::math::vec2f rdir = freq == 0.0f ? helios::math::vec2f{0.0f, 0.0f} : rightStick.normalize();

            commandBuffer.add(
                guid,
                std::make_unique<
                helios::examples::spaceshipShooting::commands::PlayerShootCommand
                >(rdir, freq)
            );
        }


    };



}