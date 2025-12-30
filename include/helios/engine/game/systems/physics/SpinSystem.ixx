/**
 * @file SpinSystem.ixx
 * @brief System for applying continuous spin rotation to entities.
 */
module;

#include <cmath>
#include <helios/engine/game/GameObjectView.h>

export module helios.engine.game.systems.physics.SpinSystem;

import helios.engine.game.System;
import helios.engine.game.UpdateContext;
import helios.engine.game.GameWorld;

import helios.math;

import helios.engine.game.components.physics.SpinComponent;
import helios.engine.game.components.physics.TransformComponent;
import helios.engine.game.components.physics.RotationStateComponent;

export namespace helios::engine::game::systems::physics {
    
    /**
     * @brief System that updates the spin rotation of entities.
     *
     * @details
     * This system iterates over entities with a SpinComponent and updates their
     * accumulated rotation angle based on the spin speed and delta time.
     * The updated rotation state is then written to the RotationStateComponent.
     */
    class SpinSystem : public System {

        /**
         * @brief Identity matrix constant.
         */
        inline static const helios::math::mat4f identity = helios::math::mat4f::identity();

    public:

        /**
         * @brief Updates spin rotation for all applicable entities.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {


            for (auto [entity, sc, rsc] : gameWorld_->find<
                helios::engine::game::components::physics::SpinComponent,
                helios::engine::game::components::physics::RotationStateComponent
            >().each()) {

                float delta = updateContext.deltaTime();

                sc->setRotation(std::fmod(sc->rotation() + sc->speed() * delta, 360.0f));

                rsc->setSpinRotationAxis(sc->axis());
                rsc->setSpinRotationAngle(sc->rotation());
            }


        }
    };
    
    
}