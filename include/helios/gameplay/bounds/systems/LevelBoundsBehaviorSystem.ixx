/**
 * @file LevelBoundsBehaviorSystem.ixx
 * @brief System for handling entity behavior at level boundaries.
 */
module;

#include <cassert>

export module helios.gameplay.bounds.systems.LevelBoundsBehaviorSystem;



import helios.math;

import helios.state.Bindings;


import helios.physics.collision.types.CollisionBehavior;
import helios.physics.collision.types.CollisionResponse;

import helios.runtime.world.GameWorld;
import helios.physics.motion.components.Move2DComponent;
import helios.physics.motion.components.SteeringComponent;
import helios.spatial.transform.components.ComposeTransformComponent;
import helios.spatial.transform.components.TranslationStateComponent;
import helios.physics.motion.components.DirectionComponent;
import helios.gameplay.bounds.components.LevelBoundsBehaviorComponent;
import helios.physics.collision.components.AabbColliderComponent;
import helios.spatial.transform.components.RotationStateComponent;
import helios.rendering.model.components.ModelAabbComponent;
import helios.scene.SceneNode;
import helios.scene.components.SceneNodeComponent;

import helios.ecs.View;

import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.gameplay.spawn.commands.DespawnCommand;

import helios.gameplay.spawn.components.SpawnedByProfileComponent;

import helios.ecs.components.Active;


import helios.runtime.world.tags.SystemRole;

using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;
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
        using Entity_type = THandle::Entity_type;
        using EngineRoleTag = helios::runtime::tags::SystemRole;
        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Updates all entities that may have left level bounds.
         *
         * @details
         * For each entity with the required components, checks if its world-space
         * AABB is within level bounds. If not, applies bounce behavior.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            using namespace helios::physics::collision::types;

            for (auto [entity, m2d, ab, sc, dc, tsc, bc, bbc, active] : updateContext.view<
                THandle,
                helios::physics::motion::components::Move2DComponent<THandle>,
                helios::rendering::model::components::ModelAabbComponent<THandle>,
                helios::scene::components::SceneNodeComponent<THandle>,
                helios::physics::motion::components::DirectionComponent<THandle>,
                helios::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::physics::collision::components::AabbColliderComponent<THandle>,
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
                        updateCollisionResponse(entity, bounceResult, bbc->collisionResponse());

                    } else if (bbc->collisionBehavior() == CollisionBehavior::Despawn) {

                        /**
                         * @todo optimize
                         */
                        auto* sbp = entity.template get<helios::gameplay::spawn::components::SpawnedByProfileComponent<THandle>>();
                        assert(sbp && "Unexpected missing SpawnProfile");

                        cmdBuffer.template add<helios::gameplay::spawn::commands::DespawnCommand<THandle>>(
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
         * @param go Pointer to the Entity to update.
         * @param bounceResult The result data from the bounce calculation.
         * @param collisionResponse The type of response to apply.
         */
        void updateCollisionResponse(
            auto entity,
            BounceResult bounceResult,
            helios::physics::collision::types::CollisionResponse collisionResponse) {

            if (collisionResponse == helios::physics::collision::types::CollisionResponse::AlignHeadingToDirection) {
                auto* psc = entity.template get<helios::physics::motion::components::SteeringComponent<THandle>>();

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
            helios::physics::motion::components::Move2DComponent<THandle>& m2d,
            helios::physics::motion::components::DirectionComponent<THandle>& dc
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
