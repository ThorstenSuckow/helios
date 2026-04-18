/**
 * @file BoundsUpdateSystem.ixx
 * @brief System for updating collider bounds based on world transforms.
 */
module;



export module helios.engine.modules.physics.collision.systems.BoundsUpdateSystem;

import helios.engine.modules.physics.collision.Bounds;


import helios.math;

import helios.core.units.Unit;

import helios.engine.runtime.world.GameObject;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.modules.scene.components.SceneNodeComponent;
import helios.engine.modules.spatial.transform.components.ScaleStateComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;
import helios.engine.modules.physics.collision.components.AabbColliderComponent;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

import helios.ecs.components.Active;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::physics::collision::systems {

    /**
     * @brief System that updates AABB colliders based on entity transforms.
     *
     * @details
     * This system recalculates the world-space AABB for entities by transforming
     * the canonical model AABB through the composed local transform (Translation * Rotation * Scale)
     * and the parent's world transform.
     *
     * Required components:
     * - ModelAabbComponent (source canonical AABB)
     * - SceneNodeComponent (for parent world transform)
     * - TranslationStateComponent (local translation)
     * - ScaleStateComponent (local scaling)
     * - RotationStateComponent (local rotation)
     * - AabbColliderComponent (receives the updated world-space AABB)
     *
     * @see helios::engine::modules::physics::collision::Bounds::computeWorldAabb()
     */
    template<typename THandle>
    class BoundsUpdateSystem {

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Updates collider bounds for all applicable entities.
         *
         * @details
         * For each entity with the required components, composes the local transform
         * from translation, rotation, and scale, then applies the parent world transform
         * to compute the final world-space AABB.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, mab, sc, tsc, sca, rsc, bc, active] : updateContext.view<
                THandle,
                helios::engine::modules::rendering::model::components::ModelAabbComponent<THandle>,
                helios::engine::modules::scene::components::SceneNodeComponent<THandle>,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::engine::modules::spatial::transform::components::ScaleStateComponent<THandle>,
                helios::engine::modules::spatial::transform::components::RotationStateComponent<THandle>,
                helios::engine::modules::physics::collision::components::AabbColliderComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                bc->setBounds(helios::engine::modules::physics::collision::Bounds::computeWorldAabb(
                    *mab, *sc, *tsc, *sca, *rsc
                ));
            }
        }

    };

}
