/**
 * @file Move2DCommand.ixx
 * @brief Command for applying directional 2D movement to a GameObject.
 */
module;

export module helios.engine.game.physics.motion.commands.Move2DCommand;

import helios.engine.game.Command;
import helios.engine.game.GameObject;
import helios.math.types;
import helios.engine.game.physics.motion.components.Move2DComponent;

export namespace helios::engine::game::physics::motion::commands {

    /**
     * @brief Command that applies 2D directional movement to a GameObject.
     *
     * @details This command encapsulates a movement request consisting of a normalized
     * direction vector and a speed factor derived from analog stick magnitude. When
     * executed, it invokes Move2DComponent::move() to update rotation target
     * and movement speed.
     *
     * @note The target GameObject must have a Move2DComponent attached for this
     *       command to have any effect.
     *
     * @see helios::engine::game::Command
     * @see helios::engine::game::physics::motion::components::Move2DComponent
     */
    class Move2DCommand : public helios::engine::game::Command {

        /**
         * @brief The analog stick magnitude determining movement intensity.
         */
        const float speedFactor_;

        /**
         * @brief The normalized direction vector for movement.
         */
        const helios::math::vec2f direction_;

    public:

        /**
         * @brief Constructs a movement command with direction and speed.
         *
         * @param direction Normalized 2D direction vector.
         * @param speedFactor Magnitude of the stick input (0.0 to 1.0).
         */
        explicit Move2DCommand(
            const helios::math::vec2f direction,
            const float speedFactor
        ) :
            direction_(direction),
            speedFactor_(speedFactor)
        {}

        /**
         * @brief Executes the movement command on a GameObject.
         *
         * @param gameObject The target entity with a Move2DComponent.
         */
        void execute(helios::engine::game::GameObject& gameObject) override {

            auto* moveComponent2D = gameObject.get<helios::engine::game::physics::motion::components::Move2DComponent>();

            if (moveComponent2D) {
                moveComponent2D->move(direction_.toVec3(), speedFactor_);
            }

        }

    };


}