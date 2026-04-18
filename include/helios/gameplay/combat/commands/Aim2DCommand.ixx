/**
 * @file Aim2DCommand.ixx
 * @brief Command for applying 2D aiming direction to GameObjects.
 */
module;

#include <cassert>

export module helios.gameplay.combat.commands.Aim2DCommand;

import helios.ecs;
import helios.math.types;
import helios.gameplay.combat.components.Aim2DComponent;

import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;
import helios.runtime.world.types;

using namespace helios::runtime::world::types;
export namespace helios::gameplay::combat::commands {

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
     * @see helios::runtime::messaging::command::Command
     * @see helios::gameplay::components::Aim2DComponent
     */
    template<typename THandle>
    class Aim2DCommand {

        /**
         * @brief The analog stick magnitude determining fire frequency.
         */
        const float freqFactor_;

        /**
         * @brief The normalized direction vector for aiming.
         */
        const helios::math::vec2f direction_;

        const THandle entityHandle_;
    public:

        /**
         * @brief Constructs an aim command with direction and frequency.
         *
         * @param direction Normalized 2D direction vector.
         * @param freqFactor Magnitude of the stick input (0.0 to 1.0).
         */
        explicit Aim2DCommand(
            const THandle entityHandle,
            const helios::math::vec2f direction,
            const float freqFactor
        ) :
        entityHandle_(entityHandle),
            direction_(direction),
            freqFactor_(freqFactor)
        {}


        /**
         * @brief Executes the aim command on a GameObject.
         *
         * @param entity The target entity with an Aim2DComponent.
         */
        void execute(helios::runtime::world::UpdateContext& updateContext) const noexcept {

            auto entity = updateContext.find<THandle>(entityHandle_);

            if (!entity) {
                assert(false && "GameObject not found in the world.");
                return;
            }

            auto* aimComponent = entity->get<helios::gameplay::combat::components::Aim2DComponent<THandle>>();

            if (aimComponent) {
                aimComponent->aim(direction_, freqFactor_);
            }

        }


    };


}