/**
 * @file ComposeTransformSystem.ixx
 * @brief System for composing final transforms from separate state components.
 */
module;



export module helios.spatial.systems.ComposeTransformSystem;



import helios.spatial.components.TranslationStateComponent;
import helios.spatial.components.RotationStateComponent;
import helios.spatial.components.ComposeTransformComponent;
import helios.gameplay.effects.gfx.components.SpinComponent;

import helios.runtime.world.GameWorld;

import helios.runtime.world.UpdateContext;

import helios.ecs.components.Active;

import helios.math;

import helios.runtime.world.tags.SystemRole;

export namespace helios::spatial::systems {

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

        using EngineRoleTag = helios::runtime::world::tags::SystemRole;

        /**
         * @brief Updates TransformComponents based on state components.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            const float deltaTime = updateContext.deltaTime();

            for (auto [entity, tc, tsc, active] : updateContext.view<
                THandle,
                helios::spatial::components::ComposeTransformComponent<THandle>,
                helios::spatial::components::TranslationStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
                tc->setLocalTranslation(tsc->translation());
            }

            for (auto [entity, tc, rsc, active] : updateContext.view<
                THandle,
                helios::spatial::components::ComposeTransformComponent<THandle>,
                helios::spatial::components::RotationStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
               tc->setLocalRotation(rsc->rotation());
            }
        }



    };

};