/**
 * @file ShootCommand.ixx
 * @brief Command for triggering shooting actions on GameObjects.
 */
module;

export module helios.engine.mechanics.combat.commands.ShootCommand;

import helios.ecs;
import helios.math.types;
import helios.engine.mechanics.combat.components.ShootComponent;
import helios.engine.modules.physics.motion.components.Move2DComponent;


import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

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
    template<typename THandle>
    class ShootCommand {

        /**
         * @brief The fire intensity factor (0.0 to 1.0).
         *
         * @details Derived from trigger pressure or button state.
         * Higher values may affect fire rate or projectile properties.
         */
        const float intensity_;


        const THandle entityHandle_;

    public:

        /**
         * @brief Constructs a ShootCommand with the specified intensity.
         *
         * @param intensity Fire intensity factor (0.0 to 1.0).
         *                  Typically from gamepad trigger pressure.
         */
        explicit ShootCommand(
            const THandle entityHandle,
            float intensity
        ) :
            entityHandle_(entityHandle),
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
        void execute(helios::engine::runtime::world::UpdateContext& updateContext) const noexcept {

            auto entity = updateContext.find<THandle>(entityHandle_);

            if (!entity) {
                return;
            }

            auto* shootComponent = entity->template get<helios::engine::mechanics::combat::components::ShootComponent<THandle>>();

            if (shootComponent) {
                auto* m2d = entity->template get<helios::engine::modules::physics::motion::components::Move2DComponent<THandle>>();

                shootComponent->shoot(
                    intensity_,
                    m2d != nullptr ? m2d->velocity() : helios::math::vec3f{0.0f, 0.0f, 0.0f}
                );
            }

        }


    };


}