/**
 * @file Aim2DCommand.ixx
 * @brief Command for applying 2D aiming direction to GameObjects.
 */
module;

export module helios.engine.game.gameplay.combat.commands.Aim2DCommand;

import helios.engine.core.messaging.command.TargetedCommand;
import helios.engine.ecs.GameObject;
import helios.math.types;
import helios.engine.game.gameplay.combat.components.Aim2DComponent;

export namespace helios::engine::game::gameplay::combat::commands {

    /**
     * @brief Command that applies 2D aiming direction to a GameObject.
     *
     * @details This command encapsulates an aim request consisting of a normalized
     * direction vector and a frequency factor derived from analog stick magnitude.
     * When executed, it invokes Aim2DComponent::aim() to update the aiming direction
     * and fire frequency.
     *
     * @note The target GameObject must have an Aim2DComponent attached for this
     *       command to have any effect.
     *
     * @see helios::engine::core::messaging::command::Command
     * @see helios::engine::game::gameplay::components::Aim2DComponent
     */
    class Aim2DCommand : public helios::engine::core::messaging::command::TargetedCommand {

        /**
         * @brief The analog stick magnitude determining fire frequency.
         */
        const float freqFactor_;

        /**
         * @brief The normalized direction vector for aiming.
         */
        const helios::math::vec2f direction_;

    public:

        /**
         * @brief Constructs an aim command with direction and frequency.
         *
         * @param direction Normalized 2D direction vector.
         * @param freqFactor Magnitude of the stick input (0.0 to 1.0).
         */
        explicit Aim2DCommand(
            const helios::math::vec2f direction,
            const float freqFactor
        ) :
            direction_(direction),
            freqFactor_(freqFactor)
        {}


        /**
         * @brief Executes the aim command on a GameObject.
         *
         * @param gameObject The target entity with an Aim2DComponent.
         */
        void execute(helios::engine::ecs::GameObject& gameObject) const noexcept override {

            auto* aimComponent = gameObject.get<helios::engine::game::gameplay::combat::components::Aim2DComponent>();

            if (aimComponent) {
                aimComponent->aim(direction_, freqFactor_);
            }

        }


    };


}