/**
 * @file BoundsUpdateSystem.ixx
 * @brief System for updating collider bounds based on world transforms.
 */
module;

#include <helios/engine/core/data/GameObjectView.h>

export module helios.engine.game.physics.collision.systems.BoundsUpdateSystem;

import helios.engine.game.System;
import helios.math;

import helios.core.units.Unit;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.engine.game.UpdateContext;

import helios.engine.game.components.scene.SceneNodeComponent;
import helios.engine.game.spatial.transform.components.ScaleComponent;
import helios.engine.game.spatial.transform.components.TranslationStateComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;
import helios.engine.game.physics.collision.components.AabbColliderComponent;

import helios.engine.game.components.model.ModelAabbComponent;

export namespace helios::engine::game::physics::collision::systems {

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
     * - ScaleComponent (local scaling)
     * - RotationStateComponent (local rotation)
     * - AabbColliderComponent (receives the updated world-space AABB)
     */
    class BoundsUpdateSystem : public System {

    public:

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
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            for (auto [entity, mab, sc, tsc, sca, rsc, bc] : gameWorld_->find<
                helios::engine::game::components::model::ModelAabbComponent,
                helios::engine::game::components::scene::SceneNodeComponent,
                helios::engine::game::spatial::transform::components::TranslationStateComponent,
                helios::engine::game::spatial::transform::components::ScaleComponent,
                helios::engine::game::physics::motion::components::RotationStateComponent,
                helios::engine::game::physics::collision::components::AabbColliderComponent
            >().each()) {

                const helios::math::mat4f& parentTransform = sc->sceneNode()->parent()->worldTransform();

                helios::math::vec3f localTranslation = tsc->translation();
                helios::math::mat4f localRotation    = rsc->rotation();
                helios::math::vec3f localScaling     = sca->scaling();

                helios::math::mat4f mScaling     = helios::math::mat4f::identity().withScaling(localScaling);
                helios::math::mat4f mTranslation = helios::math::mat4f::identity().withTranslation(localTranslation);

                helios::math::mat4f localTransform =  mTranslation * (localRotation * mScaling);

                bc->setBounds(mab->aabb().applyTransform(parentTransform * localTransform));
            }
        }

    };

}
