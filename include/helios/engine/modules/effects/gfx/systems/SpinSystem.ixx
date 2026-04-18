/**
 * @file SpinSystem.ixx
 * @brief System for applying continuous spin rotation to entities.
 */
module;


#include <cmath>

export module helios.engine.modules.physics.motion.systems.SpinSystem;


import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;

import helios.math;

import helios.engine.modules.effects.gfx.components.SpinComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;

import helios.ecs.components.Active;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::physics::motion::systems {
    
    /**
     * @brief System that updates the spin rotation of entities.
     *
     * @details
     * This system iterates over entities with a SpinComponent and updates their
     * accumulated rotation angle based on the spin speed and delta time.
     * The updated rotation state is then written to the RotationStateComponent.
     */
    template<typename THandle>
    class SpinSystem {

        /**
         * @brief Identity matrix constant.
         */
        inline static const helios::math::mat4f identity = helios::math::mat4f::identity();

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Updates spin rotation for all applicable entities.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {


            for (auto [entity, sc, rsc, active] : updateContext.view<
                THandle,
                helios::engine::modules::effects::gfx::components::SpinComponent<THandle>,
                helios::engine::modules::spatial::transform::components::RotationStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                float delta = updateContext.deltaTime();

                sc->setRotation(std::fmod(sc->rotation() + sc->speed() * delta, 360.0f));

                rsc->setSpinRotationAxis(sc->axis());
                rsc->setSpinRotationAngle(sc->rotation());
            }


        }
    };
    
    
}