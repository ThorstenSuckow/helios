/**
 * @file InputHandler.ixx
 * @brief Input handler implementation for the spaceship control example.
 */
module;

#include <memory>

export module helios.examples.spaceshipControl.InputHandler;

import helios.game.InputHandler;
import helios.input.GamepadState;
import helios.game.Command;
import helios.game.InputSnapshot;
import helios.game.CommandBuffer;
import helios.game.GameObject;
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
     * @see helios::game::InputHandler
     * @see PlayerMoveCommand
     */
    class InputHandler : public helios::game::InputHandler {

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
            const helios::game::InputSnapshot& inputSnapshot,
            const helios::util::Guid& guid,
            helios::game::CommandBuffer& commandBuffer
        ) override {

            commandBuffer.add(
                guid,
                std::make_unique<
                helios::examples::spaceshipControl::commands::PlayerMoveCommand
                >(
                    inputSnapshot.gamepadState().left().normalize(),
                    inputSnapshot.gamepadState().left().length()
                )
            );


        }


    };



}