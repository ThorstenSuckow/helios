/**
 * @file SteeringCommand.ixx
 * @brief Command for updating the heading of a GameObject.
 */
module;

export module helios.engine.modules.physics.motion.commands.SteeringCommand;

import helios.engine.ecs;
import helios.math.types;
import helios.engine.modules.physics.motion.components.SteeringComponent;


import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::modules::physics::motion::commands {


    /**
     * @brief Command that updates the heading direction of a GameObject.
     *
     * @details
     * This command encapsulates a request to change the heading (facing direction)
     * of a GameObject. It targets the SteeringComponent and applies the specified
     * direction and turn factor (interpolation speed).
     */
    class SteeringCommand  {
        

        /**
         * @brief The interpolation factor for the turn (0.0 to 1.0).
         */
        const float turnFactor_;

        /**
         * @brief The target direction vector.
         */
        const helios::math::vec2f direction_{};


        const helios::engine::ecs::EntityHandle entityHandle_;

    public:

        /**
         * @brief Constructs a new SteeringCommand.
         *
         * @param direction The target direction vector (should be normalized).
         * @param turnFactor The speed/interpolation factor for the turn.
         */
        explicit SteeringCommand(
            const helios::engine::ecs::EntityHandle entityHandle,
            const helios::math::vec2f direction,
            float turnFactor
        ) :
        entityHandle_(entityHandle),
            direction_(direction),
            turnFactor_(turnFactor)
        {}


        /**
         * @brief Executes the heading update on the target GameObject.
         *
         * @details
         * Retrieves the SteeringComponent from the GameObject and calls setHeading()
         * with the stored direction and turn factor. If the component is missing,
         * the command does nothing.
         *
         * @param gameObject The GameObject to apply the command to.
         */
        void execute(helios::engine::runtime::world::UpdateContext& updateContext) const noexcept {

            auto gameObject = updateContext.find(entityHandle_);

            if (!gameObject) {
                return;
            }

            auto* hc = gameObject->get<helios::engine::modules::physics::motion::components::SteeringComponent>();

            if (hc) {
                hc->setHeading(direction_.toVec3(), turnFactor_);
            }

        }

    };


}