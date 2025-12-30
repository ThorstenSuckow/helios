/**
 * @file ComposeTransformSystem.ixx
 * @brief System for composing final transforms from separate state components.
 */
module;

#include <helios/engine/game/GameObjectView.h>

export module helios.engine.game.systems.physics.ComposeTransformSystem;

import helios.engine.game.System;

import helios.engine.game.components.physics.TranslationStateComponent;
import helios.engine.game.components.physics.RotationStateComponent;
import helios.engine.game.components.physics.TransformComponent;
import helios.engine.game.components.physics.SpinComponent;

import helios.engine.game.GameWorld;

import helios.engine.game.UpdateContext;

import helios.math;

export namespace helios::engine::game::systems::physics {

    /**
     * @brief System that composes the final TransformComponent from state components.
     *
     * @details
     * This system takes the raw state from TranslationStateComponent and
     * RotationStateComponent and applies it to the TransformComponent.
     * It combines heading and spin rotations into a single rotation matrix
     * and updates the local translation.
     */
    class ComposeTransformSystem : public System {

    public:

        /**
         * @brief Updates TransformComponents based on state components.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            const float deltaTime = updateContext.deltaTime();

            for (auto [entity, tc, tsc] : gameWorld_->find<
                helios::engine::game::components::physics::TransformComponent,
                helios::engine::game::components::physics::TranslationStateComponent
            >().each()) {
                tc->setLocalTranslation(tsc->translation());
            }

            for (auto [entity, tc, rsc] : gameWorld_->find<
                helios::engine::game::components::physics::TransformComponent,
                helios::engine::game::components::physics::RotationStateComponent
            >().each()) {

               tc->setLocalRotation(rsc->rotation());
            }
        }



    };

};