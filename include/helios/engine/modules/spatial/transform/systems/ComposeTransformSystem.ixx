/**
 * @file ComposeTransformSystem.ixx
 * @brief System for composing final transforms from separate state components.
 */
module;



export module helios.engine.modules.spatial.transform.systems.ComposeTransformSystem;



import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;
import helios.engine.modules.effects.gfx.components.SpinComponent;

import helios.runtime.world.GameWorld;

import helios.runtime.world.UpdateContext;

import helios.ecs.components.Active;

import helios.math;

import helios.engine.common.tags.SystemRole;

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
    template<typename THandle>
    class ComposeTransformSystem {

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Updates TransformComponents based on state components.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            const float deltaTime = updateContext.deltaTime();

            for (auto [entity, tc, tsc, active] : updateContext.view<
                THandle,
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent<THandle>,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
                tc->setLocalTranslation(tsc->translation());
            }

            for (auto [entity, tc, rsc, active] : updateContext.view<
                THandle,
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent<THandle>,
                helios::engine::modules::spatial::transform::components::RotationStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
               tc->setLocalRotation(rsc->rotation());
            }
        }



    };

};