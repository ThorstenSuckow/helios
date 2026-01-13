/**
 * @file SpinSystem.ixx
 * @brief System for applying continuous spin rotation to entities.
 */
module;

#include <helios/engine/core/data/GameObjectView.h>
#include <cmath>

export module helios.engine.game.physics.motion.systems.SpinSystem;

import helios.engine.ecs.System;
import helios.engine.ecs.UpdateContext;
import helios.engine.ecs.GameWorld;

import helios.math;

import helios.engine.game.physics.motion.components.SpinComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;

export namespace helios::engine::game::physics::motion::systems {
    
    /**
     * @brief System that updates the spin rotation of entities.
     *
     * @details
     * This system iterates over entities with a SpinComponent and updates their
     * accumulated rotation angle based on the spin speed and delta time.
     * The updated rotation state is then written to the RotationStateComponent.
     */
    class SpinSystem : public helios::engine::ecs::System {

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
        void update(helios::engine::ecs::UpdateContext& updateContext) noexcept override {


            for (auto [entity, sc, rsc] : gameWorld_->find<
                helios::engine::game::physics::motion::components::SpinComponent,
                helios::engine::game::physics::motion::components::RotationStateComponent
            >().each()) {

                float delta = updateContext.deltaTime();

                sc->setRotation(std::fmod(sc->rotation() + sc->speed() * delta, 360.0f));

                rsc->setSpinRotationAxis(sc->axis());
                rsc->setSpinRotationAngle(sc->rotation());
            }


        }
    };
    
    
}