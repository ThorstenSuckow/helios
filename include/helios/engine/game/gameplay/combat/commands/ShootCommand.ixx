/**
 * @file ShootCommand.ixx
 * @brief Command for triggering shooting actions on GameObjects.
 */
module;

export module helios.engine.game.gameplay.combat.commands.ShootCommand;

import helios.engine.core.messaging.command.Command;
import helios.engine.game.GameObject;
import helios.math.types;
import helios.engine.game.gameplay.combat.components.ShootComponent;
import helios.engine.game.physics.motion.components.Move2DComponent;

export namespace helios::engine::game::gameplay::combat::commands {

    /**
     * @brief Command that triggers a shooting action on a GameObject.
     *
     * @details This command encapsulates a shoot request with an intensity factor
     * derived from trigger pressure or button state. When executed, it invokes
     * ShootComponent::shoot() to initiate projectile spawning.
     *
     * @note The target GameObject must have a ShootComponent attached for this
     *       command to have any effect.
     *
     * @see helios::engine::core::messaging::command::Command
     * @see helios::engine::game::gameplay::components::ShootComponent
     */
    class ShootCommand : public helios::engine::core::messaging::command::Command {

        /**
         * @brief The magnitude determining fire intensity.
         */
        const float intensity_;

    public:

        /**
         * @brief Constructs a shoot command with the given intensity.
         *
         * @param intensity Fire intensity factor (0.0 to 1.0).
         */
        explicit ShootCommand(
            float intensity
        ) :
            intensity_(intensity)
        {}


        /**
         * @brief Executes the shoot command on a GameObject.
         *
         * @param gameObject The target entity with a ShootComponent.
         */
        void execute(helios::engine::game::GameObject& gameObject) override {

            auto* shootComponent = gameObject.get<helios::engine::game::gameplay::combat::components::ShootComponent>();

            if (shootComponent) {
                auto* m2d            =  gameObject.get<helios::engine::game::physics::motion::components::Move2DComponent>();

                shootComponent->shoot(intensity_, m2d != nullptr ? m2d->velocity() : helios::math::vec3f{0.0f, 0.0f, 0.0f});
            }

        }


    };


}