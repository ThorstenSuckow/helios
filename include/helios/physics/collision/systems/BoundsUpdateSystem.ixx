/**
 * @file BoundsUpdateSystem.ixx
 * @brief System for updating collider bounds based on world transforms.
 */
module;



export module helios.physics.collision.systems.BoundsUpdateSystem;

import helios.physics.collision.Bounds;


import helios.math;

import helios.core.units.Unit;

import helios.runtime.world.GameObject;
import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;

import helios.scene.components.SceneNodeComponent;
import helios.spatial.transform.components.ScaleStateComponent;
import helios.spatial.transform.components.TranslationStateComponent;
import helios.spatial.transform.components.RotationStateComponent;
import helios.physics.collision.components.AabbColliderComponent;

import helios.rendering.model.components.ModelAabbComponent;

import helios.ecs.components.Active;

import helios.runtime.world.tags.SystemRole;

export namespace helios::physics::collision::systems {

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
     * @see helios::physics::collision::Bounds::computeWorldAabb()
     */
    template<typename THandle>
    class BoundsUpdateSystem {

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;

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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, mab, sc, tsc, sca, rsc, bc, active] : updateContext.view<
                THandle,
                helios::rendering::model::components::ModelAabbComponent<THandle>,
                helios::scene::components::SceneNodeComponent<THandle>,
                helios::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::spatial::transform::components::ScaleStateComponent<THandle>,
                helios::spatial::transform::components::RotationStateComponent<THandle>,
                helios::physics::collision::components::AabbColliderComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                bc->setBounds(helios::physics::collision::Bounds::computeWorldAabb(
                    *mab, *sc, *tsc, *sca, *rsc
                ));
            }
        }

    };

}
