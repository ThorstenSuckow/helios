/**
 * @file HeadingCommand.ixx
 * @brief Command for updating the heading of a GameObject.
 */
module;

export module helios.engine.game.commands.HeadingCommand;

import helios.engine.game.Command;
import helios.engine.game.GameObject;
import helios.math.types;
import helios.engine.game.components.physics.HeadingComponent;

export namespace helios::engine::game::commands {


    /**
     * @brief Command that updates the heading direction of a GameObject.
     *
     * @details
     * This command encapsulates a request to change the heading (facing direction)
     * of a GameObject. It targets the HeadingComponent and applies the specified
     * direction and turn factor (interpolation speed).
     */
    class HeadingCommand : public helios::engine::game::Command {
        

        /**
         * @brief The interpolation factor for the turn (0.0 to 1.0).
         */
        const float turnFactor_;

        /**
         * @brief The target direction vector.
         */
        const helios::math::vec2f direction_{};

    public:

        /**
         * @brief Constructs a new HeadingCommand.
         *
         * @param direction The target direction vector (should be normalized).
         * @param turnFactor The speed/interpolation factor for the turn.
         */
        explicit HeadingCommand(
            const helios::math::vec2f direction,
            float turnFactor
        ) :
            direction_(direction),
            turnFactor_(turnFactor)
        {}


        /**
         * @brief Executes the heading update on the target GameObject.
         *
         * @details
         * Retrieves the HeadingComponent from the GameObject and calls setHeading()
         * with the stored direction and turn factor. If the component is missing,
         * the command does nothing.
         *
         * @param gameObject The GameObject to apply the command to.
         */
        void execute(helios::engine::game::GameObject& gameObject) override {

            auto* hc = gameObject.get<helios::engine::game::components::physics::HeadingComponent>();

            if (hc) {
                hc->setHeading(direction_.toVec3(), turnFactor_);
            }

        }

    };


}