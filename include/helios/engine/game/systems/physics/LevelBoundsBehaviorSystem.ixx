/**
 * @file LevelBoundsBehaviorSystem.ixx
 * @brief System for handling entity behavior at level boundaries.
 */
module;

export module helios.engine.game.systems.physics.LevelBoundsBehaviorSystem;


import helios.engine.game.System;
import helios.math;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.engine.game.components.physics.Move2DComponent;
import helios.engine.game.components.physics.TransformComponent;
import helios.engine.game.components.physics.LevelBoundsBehaviorComponent;
import helios.engine.game.components.physics.AabbColliderComponent;
import helios.engine.game.components.model.ModelAabbComponent;
import helios.scene.SceneNode;
import helios.engine.game.components.scene.SceneNodeComponent;

import helios.engine.game.UpdateContext;


export namespace helios::engine::game::systems::physics {

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
            
            gameWorld_->find<
                helios::engine::game::components::physics::Move2DComponent,
                helios::engine::game::components::model::ModelAabbComponent,
                helios::engine::game::components::scene::SceneNodeComponent,
                helios::engine::game::components::physics::TransformComponent,
                helios::engine::game::components::physics::AabbColliderComponent,
                helios::engine::game::components::physics::LevelBoundsBehaviorComponent
            >().each([&](auto* entity, auto& m2d, auto& ab, auto &sc, auto &tc, auto& bc, auto& bbc) {
                
                auto objectBounds = bc.bounds();
                auto levelBounds  = gameWorld_->level().bounds();

                if (!levelBounds.contains(objectBounds)) {

                    auto worldTranslation  = tc.worldTranslation();
                    auto worldTransform = tc.worldTransform();

                    worldTranslation = bounce(worldTranslation, objectBounds, levelBounds, m2d, bbc);

                    auto& parentTransform = sc.sceneNode()->parent()->worldTransform();
                    auto parentTransform_inverse = parentTransform.inverse();

                    auto childLocalTranslation = parentTransform_inverse * worldTranslation.toVec4(1.0f);

                    tc.setLocalTranslation(childLocalTranslation.toVec3());
                    bc.setBounds(ab.aabb().applyTransform(
                        worldTransform.setTranslation(worldTranslation)
                    ));
                } 
                
            });
        }

    private:

        /**
         * @brief Applies bounce behavior to an entity that has left level bounds.
         *
         * @details
         * Calculates the corrected position and reflected velocity when an entity
         * collides with level boundaries. The restitution coefficient determines
         * how much velocity is preserved after the bounce.
         *
         * @param worldTranslation Current world position of the entity.
         * @param objectBounds World-space AABB of the entity.
         * @param levelBounds World-space AABB of the level.
         * @param m2d Reference to the Move2DComponent for velocity modification.
         * @param bbc Reference to the LevelBoundsBehaviorComponent for restitution.
         *
         * @return Corrected world position after bounce.
         */
        helios::math::vec3f bounce(
            helios::math::vec3f worldTranslation,
             helios::math::aabbf objectBounds,
             helios::math::aabbf levelBounds,
             helios::engine::game::components::physics::Move2DComponent& m2d,
             helios::engine::game::components::physics::LevelBoundsBehaviorComponent& bbc
             ){


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
            }

            return translation;
        }

    };

};
