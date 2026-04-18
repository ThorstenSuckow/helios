/**
 * @file LevelBoundsBehaviorSystem.ixx
 * @brief System for handling entity behavior at level boundaries.
 */
module;

#include <cassert>

export module helios.gameplay.bounds.systems.LevelBoundsBehaviorSystem;



import helios.math;

import helios.engine.state.Bindings;


import helios.engine.modules.physics.collision.types.CollisionBehavior;
import helios.engine.modules.physics.collision.types.CollisionResponse;

import helios.runtime.world.GameObject;
import helios.runtime.world.GameWorld;
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.motion.components.SteeringComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.modules.physics.motion.components.DirectionComponent;
import helios.gameplay.bounds.components.LevelBoundsBehaviorComponent;
import helios.engine.modules.physics.collision.components.AabbColliderComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;
import helios.engine.modules.rendering.model.components.ModelAabbComponent;
import helios.scene.SceneNode;
import helios.engine.modules.scene.components.SceneNodeComponent;

import helios.ecs.View;

import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.runtime.spawn.commands.DespawnCommand;

import helios.gameplay.spawn.components.SpawnedByProfileComponent;

import helios.ecs.components.Active;


import helios.engine.common.tags.SystemRole;

using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;
export namespace helios::gameplay::bounds::systems {

    /**
     * @brief System that handles entity behavior when colliding with level boundaries.
     *
     * @details
     * This system checks if entities with movement components have exceeded the level bounds.
     * When an entity leaves the bounds, it applies bounce behavior based on the
     * LevelBoundsBehaviorComponent's restitution coefficient.
     */
        template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
        requires IsCommandBufferLike<TCommandBuffer>
    class LevelBoundsBehaviorSystem {

        /**
         * @brief Result of a bounce calculation against level bounds.
         */
        struct BounceResult {
            /** @brief True if the X-axis boundary was hit. */
            bool hitX;
            /** @brief True if the Y-axis boundary was hit. */
            bool hitY;
            /** @brief The corrected world position. */
            helios::math::vec3f translation;
            /** @brief The new velocity after bounce. */
            helios::math::vec3f velocity;
            /** @brief The new direction vector. */
            helios::math::vec3f direction;
        };


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Updates all entities that may have left level bounds.
         *
         * @details
         * For each entity with the required components, checks if its world-space
         * AABB is within level bounds. If not, applies bounce behavior.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            using namespace helios::engine::modules::physics::collision::types;

            for (auto [entity, m2d, ab, sc, dc, tsc, bc, bbc, active] : updateContext.view<
                THandle,
                helios::engine::modules::physics::motion::components::Move2DComponent<THandle>,
                helios::engine::modules::rendering::model::components::ModelAabbComponent<THandle>,
                helios::engine::modules::scene::components::SceneNodeComponent<THandle>,
                helios::engine::modules::physics::motion::components::DirectionComponent<THandle>,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::engine::modules::physics::collision::components::AabbColliderComponent<THandle>,
                helios::gameplay::bounds::components::LevelBoundsBehaviorComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {


                auto& objectBounds = bc->bounds();
                auto levelBounds  = updateContext.level()->bounds();

                if (!levelBounds.contains(objectBounds)) {

                    helios::math::mat4f parentWorldTransform = sc->sceneNode()->parent()->worldTransform();
                    helios::math::vec4f childWorldTranslation = parentWorldTransform * tsc->translation().toVec4(1.0f);

                    helios::math::vec3f bouncedWorldTranslation;

                    BounceResult bounceResult{};

                    if (bbc->collisionBehavior() == CollisionBehavior::Reflect || bbc->collisionBehavior() == CollisionBehavior::Bounce) {

                        bounceResult = bounce(
                           childWorldTranslation.toVec3(), objectBounds, levelBounds,
                           bbc->collisionBehavior() == CollisionBehavior::Reflect ? 1.0f : bbc->restitution(),
                           *m2d,  *dc
                        );

                        bouncedWorldTranslation = bounceResult.translation;
                        updateCollisionResponse(entity.handle(), bounceResult, bbc->collisionResponse());

                    } else if (bbc->collisionBehavior() == CollisionBehavior::Despawn) {

                        /**
                         * @todo optimize
                         */
                        auto* sbp = entity.get<helios::gameplay::spawn::components::SpawnedByProfileComponent<THandle>>();
                        assert(sbp && "Unexpected missing SpawnProfile");

                        updateContext.queueCommand<TCommandBuffer, helios::runtime::spawn::commands::DespawnCommand<THandle>>(
                            entity.handle(), sbp->spawnProfileId()
                        );

                    }


                    if (bounceResult.hitX || bounceResult.hitY) {
                        m2d->setVelocity(bounceResult.velocity);
                        dc->setDirection(bounceResult.direction);
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
         * @brief Updates the entity's state based on the configured collision response.
         *
         * @details
         * If the response is set to AlignHeadingToDirection, this method updates the
         * SteeringComponent to match the new bounce direction.
         *
         * @param go Pointer to the GameObject to update.
         * @param bounceResult The result data from the bounce calculation.
         * @param collisionResponse The type of response to apply.
         */
        void updateCollisionResponse(
            helios::runtime::world::GameObject go,
            BounceResult bounceResult,
            helios::engine::modules::physics::collision::types::CollisionResponse collisionResponse) {

            if (collisionResponse == helios::engine::modules::physics::collision::types::CollisionResponse::AlignHeadingToDirection) {
                auto* psc = go.get<helios::engine::modules::physics::motion::components::SteeringComponent<THandle>>();

                const auto direction = bounceResult.direction;

                if (psc) {
                    psc->setSteeringIntent(
                        bounceResult.direction, 1.0f
                    );
                }
            }
        }

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
        [[nodiscard]] BounceResult bounce(
            helios::math::vec3f worldTranslation,
            helios::math::aabbf objectBounds,
            helios::math::aabbf levelBounds,
            const float restitution,
            helios::engine::modules::physics::motion::components::Move2DComponent<THandle>& m2d,
            helios::engine::modules::physics::motion::components::DirectionComponent<THandle>& dc
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


            return {
                hitX, hitY, translation, velocity, velocity.normalize()
            };
        }

    };

};
