/**
 * @file ComposeTransformSystem.ixx
 * @brief System for composing final transforms from separate state components.
 */
module;



export module helios.engine.modules.spatial.transform.systems.ComposeTransformSystem;

import helios.engine.ecs.System;

import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;
import helios.engine.modules.effects.gfx.components.SpinComponent;

import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.math;

export namespace helios::engine::modules::spatial::transform::systems {

    /**
     * @brief System that composes the final ComposeTransformComponent from state components.
     *
     * @details
     * This system takes the raw state from TranslationStateComponent and
     * RotationStateComponent and applies it to the ComposeTransformComponent.
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

            for (auto [entity, tc, tsc, active] : gameWorld_->view<
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {
                tc->setLocalTranslation(tsc->translation());
            }

            for (auto [entity, tc, rsc, active] : gameWorld_->view<
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
                helios::engine::modules::spatial::transform::components::RotationStateComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

               tc->setLocalRotation(rsc->rotation());
            }
        }



    };

};