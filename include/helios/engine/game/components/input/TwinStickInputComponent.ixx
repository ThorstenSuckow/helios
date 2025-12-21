/**
 * @file TwinStickInputComponent.ixx
 * @brief Input component for twin-stick gamepad controls.
 */
module;

#include <memory>

export module helios.engine.game.components.input.TwinStickInputComponent;

import helios.math.types;
import helios.math.utils;
import helios.engine.game.GameObject;
import helios.engine.game.UpdateContext;
import helios.engine.game.Updatable;
import helios.engine.game.Component;
import helios.engine.game.CommandBuffer;
import helios.engine.game.commands.Move2DCommand;
import helios.engine.game.commands.Aim2DCommand;
import helios.engine.game.commands.ShootCommand;

export namespace helios::engine::game::components::input {

    /**
     * @brief Input component for twin-stick gamepad control schemes.
     *
     * @details Translates gamepad analog stick input into movement and aiming commands.
     * The left stick controls movement direction and speed, while the right stick
     * controls aiming direction and intensity.
     *
     * Each frame, this component reads the current input snapshot and generates
     * Move2DCommand and Aim2DCommand instances that are queued in the CommandBuffer
     * for later execution.
     *
     * @note Requires the owning GameObject to have Move2DComponent and Aim2DComponent
     *       attached for the generated commands to have any effect.
     */
    class TwinStickInputComponent : public helios::engine::game::Updatable, public helios::engine::game::Component {

        /**
         * @brief Flag to indicate whether shoot commands should be derived
         * from the aim component.
         *
         * If true, ShootCommands will be created from dedicated input.
         */
        bool useDedicatedShootInput_ = true;

    public:

        /**
         * @brief Processes gamepad input and generates movement/aiming commands.
         *
         * @param updateContext Context containing input snapshot and command buffer.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            auto* inputSnapshot = updateContext.inputSnapshot();
            auto* commandBuffer = updateContext.commandBuffer();

            // Left stick: movement
            const auto leftStick = inputSnapshot->gamepadState().left();
            float finalSpeed = 0.0f;
            float speed = leftStick.length();
            auto ldir = helios::math::vec2f{0.0f, 0.0f};

            if (speed > helios::math::EPSILON_LENGTH) {
                ldir = leftStick.normalize();
                finalSpeed = speed;
            }

            commandBuffer->add(
                gameObject()->guid(),
                std::make_unique<helios::engine::game::commands::Move2DCommand>(ldir, finalSpeed)
            );

            // Right stick: aiming
            const auto rightStick = inputSnapshot->gamepadState().right();
            float freq = rightStick.length();
            float finalFreq = 0.0f;
            auto rdir = helios::math::vec2f{0.0f, 0.0f};

            if (freq > helios::math::EPSILON_LENGTH) {
                rdir = rightStick.normalize();
                finalFreq = freq;
            }

            commandBuffer->add(
                gameObject()->guid(),
                std::make_unique<helios::engine::game::commands::Aim2DCommand>(rdir, finalFreq)
            );

            if (useDedicatedShootInput_) {
                // right trigger: shooting
                const auto rightTrigger = inputSnapshot->gamepadState().triggerRight();

                commandBuffer->add(
                   gameObject()->guid(),
                   std::make_unique<helios::engine::game::commands::ShootCommand>(rightTrigger)
               );
            } else {
                commandBuffer->add(
                   gameObject()->guid(),
                   std::make_unique<helios::engine::game::commands::ShootCommand>(finalFreq)
               );
            }



        };


    };


}