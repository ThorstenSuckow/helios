/**
 * @file ComposeTransformSystem.ixx
 * @brief System for composing final transforms from separate state components.
 */
module;

#include <helios/engine/ecs/query/GameObjectView.h>

export module helios.engine.game.spatial.transform.systems.ComposeTransformSystem;

import helios.engine.ecs.System;

import helios.engine.game.spatial.transform.components.TranslationStateComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;
import helios.engine.game.physics.motion.components.SpinComponent;

import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.UpdateContext;

import helios.math;

export namespace helios::engine::game::spatial::transform::systems {

    /**
     * @brief System that composes the final TransformComponent from state components.
     *
     * @details
     * This system takes the raw state from TranslationStateComponent and
     * RotationStateComponent and applies it to the TransformComponent.
     * It combines heading and spin rotations into a single rotation matrix
     * and updates the local translation.
     */
    class ComposeTransformSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Updates TransformComponents based on state components.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            const float deltaTime = updateContext.deltaTime();

            for (auto [entity, tc, tsc] : gameWorld_->find<
                helios::engine::game::spatial::transform::components::TransformComponent,
                helios::engine::game::spatial::transform::components::TranslationStateComponent
            >().each()) {
                tc->setLocalTranslation(tsc->translation());
            }

            for (auto [entity, tc, rsc] : gameWorld_->find<
                helios::engine::game::spatial::transform::components::TransformComponent,
                helios::engine::game::physics::motion::components::RotationStateComponent
            >().each()) {

               tc->setLocalRotation(rsc->rotation());
            }
        }



    };

};