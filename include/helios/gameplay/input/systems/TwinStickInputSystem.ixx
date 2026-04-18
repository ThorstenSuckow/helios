/**
 * @file TwinStickInputSystem.ixx
 * @brief Input component for twin-stick gamepad controls.
 */
module;

#include <memory>

export module helios.gameplay.input.systems.TwinStickInputSystem;

import helios.math.types;
import helios.math.utils;
import helios.runtime.world.GameObject;
import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;


import helios.engine.state.Bindings;


import helios.gameplay.lifecycle.components.DeadTagComponent;

import helios.engine.modules.physics.motion.commands.Move2DCommand;
import helios.engine.modules.physics.motion.commands.SteeringCommand;
import helios.gameplay.combat.commands.Aim2DCommand;
import helios.gameplay.combat.commands.ShootCommand;

import helios.engine.common.tags.SystemRole;


using namespace helios::gameplay::lifecycle::components;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;
export namespace helios::gameplay::input::systems {

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
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class TwinStickInputSystem {

        /**
         * @brief Flag to indicate whether shoot commands should be derived
         * from the aim component.
         *
         * If true, ShootCommands will be created from dedicated input.
         */
        bool useDedicatedShootInput_ = false;

        /**
         * @brief Reference to the EntityHandle this system reads input for.
         */
        THandle entityHandle_;

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Constructs a TwinStickInputSystem for the specified GameObject.
         *
         * @param gameObject Reference to the GameObject to generate input commands for.
         */
        explicit TwinStickInputSystem(THandle entityHandle) :
        entityHandle_(entityHandle) {}

        /**
         * @brief Processes gamepad input and generates movement/aiming commands.
         *
         * @param updateContext Context containing input snapshot and command buffer.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            auto& inputSnapshot = updateContext.inputSnapshot();

            // Left stick: movement
            const auto leftStick = inputSnapshot.gamepadState().left();
            float finalSpeed = 0.0f;
            float speed = leftStick.length();
            auto ldir = helios::math::vec2f{0.0f, 0.0f};

            // Right stick: aiming
            const auto rightStick = inputSnapshot.gamepadState().right();
            float freq = rightStick.length();
            float finalFreq = 0.0f;
            auto rdir = helios::math::vec2f{0.0f, 0.0f};

            if (entityHandle_.has<DeadTagComponent>()) {
                updateContext.queueCommand<TCommandBuffer, helios::engine::modules::physics::motion::commands::Move2DCommand<THandle>>(
                    entityHandle_, ldir, finalSpeed
                );
                updateContext.queueCommand<TCommandBuffer, helios::gameplay::combat::commands::Aim2DCommand<THandle>>(
                    entityHandle_, rdir, finalFreq
                );
                return;
            }

            if (speed > helios::math::EPSILON_LENGTH) {
                ldir = leftStick.normalize();
                finalSpeed = speed;
            }
            /**
             * @todo DO NOT POST IF input is already inactive in shootComponent
             * and no input was detected (after normalizing)
             */
            updateContext.queueCommand<TCommandBuffer, helios::engine::modules::physics::motion::commands::Move2DCommand<THandle>>(
                entityHandle_, ldir, finalSpeed
            );

            updateContext.queueCommand<TCommandBuffer, helios::engine::modules::physics::motion::commands::SteeringCommand<THandle>>(
                entityHandle_, ldir, finalSpeed
            );

            if (freq > helios::math::EPSILON_LENGTH) {
                rdir = rightStick.normalize();
                finalFreq = freq;
            }

            updateContext.queueCommand<TCommandBuffer, helios::gameplay::combat::commands::Aim2DCommand<THandle>>(
                entityHandle_, rdir, finalFreq
            );

            if (useDedicatedShootInput_) {
                // right trigger: shooting
                const auto rightTrigger = inputSnapshot.gamepadState().triggerRight();
                if (rightTrigger > 0.0f) {
                    updateContext.queueCommand<TCommandBuffer, helios::gameplay::combat::commands::ShootCommand<THandle>>(
                        entityHandle_, rightTrigger
                    );
                }
            } else {
                if (finalFreq > 0.0f) {
                    updateContext.queueCommand<TCommandBuffer, helios::gameplay::combat::commands::ShootCommand<THandle>>(
                        entityHandle_, finalFreq
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