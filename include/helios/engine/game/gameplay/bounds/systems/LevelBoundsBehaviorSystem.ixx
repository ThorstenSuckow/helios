/**
 * @file LevelBoundsBehaviorSystem.ixx
 * @brief System for handling entity behavior at level boundaries.
 */
module;

#include <helios/engine/core/data/GameObjectView.h>

export module helios.engine.game.gameplay.bounds.systems.LevelBoundsBehaviorSystem;


import helios.engine.game.System;
import helios.math;

import helios.engine.core.messaging.command.CommandBuffer;

import helios.engine.game.physics.collision.types.CollisionBehavior;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.engine.game.physics.motion.components.Move2DComponent;
import helios.engine.game.physics.motion.components.SteeringComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;
import helios.engine.game.spatial.transform.components.TranslationStateComponent;
import helios.engine.game.physics.motion.components.DirectionComponent;
import helios.engine.game.gameplay.bounds.components.LevelBoundsBehaviorComponent;
import helios.engine.game.physics.collision.components.AabbColliderComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;
import helios.engine.game.model.components.ModelAabbComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;
import helios.scene.SceneNode;
import helios.engine.game.scene.components.SceneNodeComponent;

import helios.engine.game.UpdateContext;

import helios.engine.game.gameplay.spawn.commands.DespawnCommand;

import helios.engine.game.pool.components.PoolIdComponent;

export namespace helios::engine::game::gameplay::bounds::systems {

    /**
     * @brief System that handles entity behavior when colliding with level boundaries.
     *
     * @details
     * This system checks if entities with movement components have exceeded the level bounds.
     * When an entity leaves the bounds, it applies bounce behavior based on the
     * LevelBoundsBehaviorComponent's restitution coefficient.
     */
    class LevelBoundsBehaviorSystem : public System {

    public:

        /**
         * @brief Updates all entities that may have left level bounds.
         *
         * @details
         * For each entity with the required components, checks if its world-space
         * AABB is within level bounds. If not, applies bounce behavior.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            using namespace helios::engine::game::physics::collision::types;

            for (auto [entity, m2d, ab, sc, dc, tsc, bc, bbc, rsc] : gameWorld_->find<
                helios::engine::game::physics::motion::components::Move2DComponent,
                helios::engine::game::model::components::ModelAabbComponent,
                helios::engine::game::scene::components::SceneNodeComponent,
                helios::engine::game::physics::motion::components::DirectionComponent,
                helios::engine::game::spatial::transform::components::TranslationStateComponent,
                helios::engine::game::physics::collision::components::AabbColliderComponent,
                helios::engine::game::gameplay::bounds::components::LevelBoundsBehaviorComponent,
                helios::engine::game::physics::motion::components::RotationStateComponent
            >().each()) {


                auto& objectBounds = bc->bounds();
                auto levelBounds  = gameWorld_->level().bounds();

                if (!levelBounds.contains(objectBounds)) {

                    helios::math::mat4f parentWorldTransform = sc->sceneNode()->parent()->worldTransform();
                    helios::math::vec4f childWorldTranslation = parentWorldTransform * tsc->translation().toVec4(1.0f);

                    helios::math::vec3f bouncedWorldTranslation;


                    if (bbc->collisionBehavior() == CollisionBehavior::Reflect || bbc->collisionBehavior() == CollisionBehavior::Bounce) {
                        bouncedWorldTranslation = bounce(
                           childWorldTranslation.toVec3(), objectBounds, levelBounds,
                           bbc->collisionBehavior() == CollisionBehavior::Reflect ? 1.0f : bbc->restitution(),
                           *m2d,  *dc, *rsc
                       );
                    } else if (bbc->collisionBehavior() == CollisionBehavior::Despawn) {
                        /**
                         * @todo optimize
                         */
                        auto* pic = entity->get<helios::engine::game::pool::components::PoolIdComponent>();
                        if (!pic) {
                            updateContext.commandBuffer().add<helios::engine::game::gameplay::spawn::commands::DespawnCommand>(
                                entity->guid()
                            );
                        } else {
                            updateContext.commandBuffer().add<helios::engine::game::gameplay::spawn::commands::DespawnCommand>(
                                entity->guid(), pic->gameObjectPoolId()
                            );
                        }
                    }

                    auto parentTransform_inverse = parentWorldTransform.inverse();
                    auto childLocalTranslation = parentTransform_inverse * bouncedWorldTranslation.toVec4(1.0f);

                    tsc->setTranslation(childLocalTranslation.toVec3());
                    bc->setBounds(ab->aabb().applyTransform(
                        parentWorldTransform.withTranslation(bouncedWorldTranslation)
                    ));

                }
            }
        }

    private:

        /**
         * @brief Applies bounce behavior to an entity that has left level bounds.
         *
         * @details
         * Calculates the corrected position and reflected velocity when an entity
         * collides with level boundaries. The restitution coefficient determines
         * how much velocity is preserved after the bounce. The DirectionComponent
         * will be updated with the new direction.
         *
         * @param worldTranslation Current world position of the entity.
         * @param objectBounds World-space AABB of the entity.
         * @param levelBounds World-space AABB of the level.
         * @param restitution
         * @param m2d Reference to the Move2DComponent for velocity modification.
         * @param dc Reference to the DirectionComponent for direction update.
         *
         * @return Corrected world position after bounce.
         */
        [[nodiscard]] static helios::math::vec3f bounce(
            helios::math::vec3f worldTranslation,
            helios::math::aabbf objectBounds,
            helios::math::aabbf levelBounds,
            const float restitution,
            helios::engine::game::physics::motion::components::Move2DComponent& m2d,
            helios::engine::game::physics::motion::components::DirectionComponent& dc,
            helios::engine::game::physics::motion::components::RotationStateComponent& rsc
        ) noexcept {


            auto velocity = m2d.velocity();
            bool hitX = false;
            bool hitY = false;
            auto translation = worldTranslation;

            // Check left boundary collision
            if (objectBounds.min()[0] < levelBounds.min()[0]) {
                translation[0] += (levelBounds.min()[0] - objectBounds.min()[0]);
                if (velocity[0] < 0) {
                    velocity = (velocity.withX(restitution * -velocity[0]));
                }
                hitX = true;
            // Check right boundary collision
            } else  if (objectBounds.max()[0] > levelBounds.max()[0]) {
                translation[0] -= (objectBounds.max()[0] - levelBounds.max()[0]) ;
                if (velocity[0] > 0) {
                    velocity = (velocity.withX(restitution * -velocity[0]));
                }
                hitX = true;
            }

            objectBounds = objectBounds + (translation - worldTranslation);

            // Check bottom boundary collision
            if (objectBounds.min()[1] < levelBounds.min()[1]) {
                translation[1] += (levelBounds.min()[1] - objectBounds.min()[1]);
                if (velocity[1] < 0) {
                    velocity = velocity.withY(restitution * -velocity[1]);
                }
                hitY = true;
            // Check top boundary collision
            } else if (objectBounds.max()[1] > levelBounds.max()[1]) {
                translation[1] -= (objectBounds.max()[1] - levelBounds.max()[1]);
                if (velocity[1] > 0) {
                    velocity = velocity.withY(restitution * -velocity[1]);
                }
                hitY = true;
            }

            if (hitX || hitY) {
                m2d.setVelocity(velocity);
                dc.setDirection(velocity.normalize());
            }

            return translation;
        }

    };

};
