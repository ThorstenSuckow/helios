/**
 * @file ShootCommand.ixx
 * @brief Command for triggering shooting actions on GameObjects.
 */
module;

export module helios.engine.mechanics.combat.commands.ShootCommand;

import helios.engine.runtime.messaging.command.TargetedCommand;
import helios.engine.ecs.GameObject;
import helios.math.types;
import helios.engine.mechanics.combat.components.ShootComponent;
import helios.engine.modules.physics.motion.components.Move2DComponent;

import helios.engine.runtime.messaging.command.TypedTargetedCommandDispatcher;
import helios.engine.runtime.messaging.command.TargetedCommandDispatcher;

export namespace helios::engine::mechanics::combat::commands {

    /**
     * @brief Command that triggers a shooting action on a GameObject.
     *
     * @details ShootCommand is a TargetedCommand that initiates projectile firing
     * on the target entity. The command carries an intensity value (typically from
     * trigger pressure or button state) that controls fire rate or projectile behavior.
     *
     * When executed, the command:
     * 1. Retrieves the target's ShootComponent
     * 2. Optionally retrieves Move2DComponent for velocity inheritance
     * 3. Calls `ShootComponent::shoot()` with intensity and source velocity
     *
     * The source velocity allows projectiles to inherit the shooter's momentum,
     * preventing projectiles from spawning "behind" a fast-moving entity.
     *
     * Example usage:
     * ```cpp
     * // In an input system
     * float triggerValue = inputSnapshot.gamepadState().triggerRight();
     * if (triggerValue > 0.0f) {
     *     commandBuffer.add<ShootCommand>(player.guid(), triggerValue);
     * }
     * ```
     *
     * @note The target GameObject must have a ShootComponent attached for this
     *       command to have any effect. Move2DComponent is optional.
     *
     * @see ShootComponent
     * @see TargetedCommand
     * @see TwinStickInputSystem
     */
    class ShootCommand : public helios::engine::runtime::messaging::command::TargetedCommand {

        /**
         * @brief The fire intensity factor (0.0 to 1.0).
         *
         * @details Derived from trigger pressure or button state.
         * Higher values may affect fire rate or projectile properties.
         */
        const float intensity_;

    public:

        /**
         * @brief Constructs a ShootCommand with the specified intensity.
         *
         * @param intensity Fire intensity factor (0.0 to 1.0).
         *                  Typically from gamepad trigger pressure.
         */
        explicit ShootCommand(
            float intensity
        ) :
            intensity_(intensity)
        {}

        /**
         * @brief Returns the fire intensity.
         *
         * @return The intensity value (0.0 to 1.0).
         */
        [[nodiscard]] float intensity() const noexcept {
            return intensity_;
        }

        /**
         * @brief Executes the shoot command on the target GameObject.
         *
         * @details Retrieves the ShootComponent and optionally the Move2DComponent
         * from the target. Calls `ShootComponent::shoot()` with the intensity and
         * the entity's current velocity for momentum inheritance.
         *
         * @param gameObject The target entity with a ShootComponent.
         */
        void execute(helios::engine::ecs::GameObject& gameObject) const noexcept override {

            auto* shootComponent = gameObject.get<helios::engine::mechanics::combat::components::ShootComponent>();

            if (shootComponent) {
                auto* m2d = gameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();

                shootComponent->shoot(
                    intensity_,
                    m2d != nullptr ? m2d->velocity() : helios::math::vec3f{0.0f, 0.0f, 0.0f}
                );
            }

        }

        /**
         * @brief Accepts a dispatcher for command routing.
         *
         * @param gameObject The target GameObject.
         * @param dispatcher The dispatcher to route this command to.
         */
        void accept(
            helios::engine::ecs::GameObject& gameObject,
            helios::engine::runtime::messaging::command::TargetedCommandDispatcher& dispatcher
        ) const noexcept override {
            dispatcher.dispatch(gameObject, *this);
        }


    };


}