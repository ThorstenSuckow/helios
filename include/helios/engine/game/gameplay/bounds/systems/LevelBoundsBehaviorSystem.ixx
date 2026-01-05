/**
 * @file LevelBoundsBehaviorSystem.ixx
 * @brief System for handling entity behavior at level boundaries.
 */
module;

#include <helios/engine/game/GameObjectView.h>

export module helios.engine.game.gameplay.bounds.systems.LevelBoundsBehaviorSystem;


import helios.engine.game.System;
import helios.math;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.engine.game.physics.motion.components.Move2DComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;
import helios.engine.game.spatial.transform.components.TranslationStateComponent;
import helios.engine.game.physics.motion.components.DirectionComponent;
import helios.engine.game.gameplay.bounds.components.LevelBoundsBehaviorComponent;
import helios.engine.game.physics.collision.components.AabbColliderComponent;
import helios.engine.game.components.model.ModelAabbComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;
import helios.scene.SceneNode;
import helios.engine.game.components.scene.SceneNodeComponent;

import helios.engine.game.UpdateContext;


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

            for (auto [entity, m2d, ab, sc, dc, tsc, bc, bbc] : gameWorld_->find<
                helios::engine::game::physics::motion::components::Move2DComponent,
                helios::engine::game::components::model::ModelAabbComponent,
                helios::engine::game::components::scene::SceneNodeComponent,
                helios::engine::game::physics::motion::components::DirectionComponent,
                helios::engine::game::spatial::transform::components::TranslationStateComponent,
                helios::engine::game::physics::collision::components::AabbColliderComponent,
                helios::engine::game::gameplay::bounds::components::LevelBoundsBehaviorComponent
            >().each()) {


                auto& objectBounds = bc->bounds();
                auto levelBounds  = gameWorld_->level().bounds();

                if (!levelBounds.contains(objectBounds)) {

                    helios::math::mat4f parentWorldTransform = sc->sceneNode()->parent()->worldTransform();
                    helios::math::vec4f childWorldTranslation = parentWorldTransform * tsc->translation().toVec4(1.0f);

                    auto bouncedWorldTranslation = bounce(
                        childWorldTranslation.toVec3(), objectBounds, levelBounds, *m2d, *bbc, *dc
                    );

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
         * @param m2d Reference to the Move2DComponent for velocity modification.
         * @param bbc Reference to the LevelBoundsBehaviorComponent for restitution.
         * @param dc Reference to the DirectionComponent for direction update.
         *
         * @return Corrected world position after bounce.
         */
        [[nodiscard]] static helios::math::vec3f bounce(
            helios::math::vec3f worldTranslation,
            helios::math::aabbf objectBounds,
            helios::math::aabbf levelBounds,
            helios::engine::game::physics::motion::components::Move2DComponent& m2d,
            helios::engine::game::gameplay::bounds::components::LevelBoundsBehaviorComponent& bbc,
            helios::engine::game::physics::motion::components::DirectionComponent& dc
        ) noexcept {


            auto velocity = m2d.velocity();
            bool hitX = false;
            bool hitY = false;
            auto translation = worldTranslation;

            // Check left boundary collision
            if (objectBounds.min()[0] < levelBounds.min()[0]) {
                translation[0] += (levelBounds.min()[0] - objectBounds.min()[0]);
                if (velocity[0] < 0) {
                    velocity = (velocity.withX(bbc.restitution() * -velocity[0]));
                }
                hitX = true;
            // Check right boundary collision
            } else  if (objectBounds.max()[0] > levelBounds.max()[0]) {
                translation[0] -= (objectBounds.max()[0] - levelBounds.max()[0]) ;
                if (velocity[0] > 0) {
                    velocity = (velocity.withX(bbc.restitution() * -velocity[0]));
                }
                hitX = true;
            }

            objectBounds = objectBounds + (translation - worldTranslation);

            // Check bottom boundary collision
            if (objectBounds.min()[1] < levelBounds.min()[1]) {
                translation[1] += (levelBounds.min()[1] - objectBounds.min()[1]);
                if (velocity[1] < 0) {
                    velocity = velocity.withY(bbc.restitution() * -velocity[1]);
                }
                hitY = true;
            // Check top boundary collision
            } else if (objectBounds.max()[1] > levelBounds.max()[1]) {
                translation[1] -= (objectBounds.max()[1] - levelBounds.max()[1]);
                if (velocity[1] > 0) {
                    velocity = velocity.withY(bbc.restitution() * -velocity[1]);
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
