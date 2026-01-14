/**
 * @file TwinStickInputSystem.ixx
 * @brief Input component for twin-stick gamepad controls.
 */
module;

#include <memory>

export module helios.engine.game.input.systems.TwinStickInputSystem;

import helios.math.types;
import helios.math.utils;
import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.System;
import helios.engine.ecs.Component;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.game.physics.motion.commands.Move2DCommand;
import helios.engine.game.physics.motion.commands.SteeringCommand;
import helios.engine.mechanics.combat.commands.Aim2DCommand;
import helios.engine.mechanics.combat.commands.ShootCommand;

export namespace helios::engine::game::input::systems {

    /**
     * @brief Input component for twin-stick gamepad control schemes.
     *
     * @details Translates gamepad analog stick input into movement and aiming commands.
     * The left stick controls movement direction and speed, while the right stick
     * controls aiming direction and intensity.
     *
     * Each frame, this component reads the current input snapshot and generates
     * Move2DCommand, SteeringCommand and Aim2DCommand instances that are queued in the CommandBuffer
     * for later execution.
     *
     * @note Requires the owning GameObject to have Move2DComponent and Aim2DComponent
     *       attached for the generated commands to have any effect.
     */
    class TwinStickInputSystem : public helios::engine::ecs::System {

        /**
         * @brief Flag to indicate whether shoot commands should be derived
         * from the aim component.
         *
         * If true, ShootCommands will be created from dedicated input.
         */
        bool useDedicatedShootInput_ = false;

        /**
         * @brief Reference to the GameObject this system reads input for.
         */
        helios::engine::ecs::GameObject& gameObject_;

    public:

        /**
         * @brief Constructs a TwinStickInputSystem for the specified GameObject.
         *
         * @param gameObject Reference to the GameObject to generate input commands for.
         */
        explicit TwinStickInputSystem(helios::engine::ecs::GameObject& gameObject) :
        gameObject_(gameObject) {}

        /**
         * @brief Processes gamepad input and generates movement/aiming commands.
         *
         * @param updateContext Context containing input snapshot and command buffer.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            auto& inputSnapshot = updateContext.inputSnapshot();
            auto& commandBuffer = updateContext.commandBuffer();

            // Left stick: movement
            const auto leftStick = inputSnapshot.gamepadState().left();
            float finalSpeed = 0.0f;
            float speed = leftStick.length();
            auto ldir = helios::math::vec2f{0.0f, 0.0f};

            if (speed > helios::math::EPSILON_LENGTH) {
                ldir = leftStick.normalize();
                finalSpeed = speed;
            }
            /**
             * @todo DO NOT POST IF input is already inactive in shootComponent
             * and no input was detected (after normalizing)
             */
            commandBuffer.add<helios::engine::game::physics::motion::commands::Move2DCommand>(
                gameObject_.guid(), ldir, finalSpeed
            );

            commandBuffer.add<helios::engine::game::physics::motion::commands::SteeringCommand>(
                gameObject_.guid(), ldir, finalSpeed
            );

            // Right stick: aiming
            const auto rightStick = inputSnapshot.gamepadState().right();
            float freq = rightStick.length();
            float finalFreq = 0.0f;
            auto rdir = helios::math::vec2f{0.0f, 0.0f};

            if (freq > helios::math::EPSILON_LENGTH) {
                rdir = rightStick.normalize();
                finalFreq = freq;
            }

            commandBuffer.add<helios::engine::mechanics::combat::commands::Aim2DCommand>(
                gameObject_.guid(), rdir, finalFreq
            );

            if (useDedicatedShootInput_) {
                // right trigger: shooting
                const auto rightTrigger = inputSnapshot.gamepadState().triggerRight();
                if (rightTrigger > 0.0f) {
                    commandBuffer.add<helios::engine::mechanics::combat::commands::ShootCommand>(
                       gameObject_.guid(), rightTrigger
                   );
                }
            } else {
                if (finalFreq > 0.0f) {
                    commandBuffer.add<helios::engine::mechanics::combat::commands::ShootCommand>(
                       gameObject_.guid(), finalFreq
                   );
                }
            }



        }

        /**
         * @brief Enables or disables dedicated shoot input mode.
         *
         * @param useDedicatedInput If true, ShootCommands use the right trigger.
         *                          If false, the aim stick magnitude is used.
         */
        void setUseDedicatedShootInput(bool useDedicatedInput) noexcept {
            useDedicatedShootInput_ = useDedicatedInput;
        }

        /**
         * @brief Returns whether dedicated shoot input mode is enabled.
         *
         * @return True if ShootCommands use the right trigger, false if aim magnitude is used.
         */
        [[nodiscard]] bool useDedicatedShootInput() const noexcept {
            return useDedicatedShootInput_;
        }

    };


}