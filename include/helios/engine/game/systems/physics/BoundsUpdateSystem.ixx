/**
 * @file BoundsUpdateSystem.ixx
 * @brief System for updating collider bounds based on world transforms.
 */
module;


export module helios.engine.game.systems.physics.BoundsUpdateSystem;

import helios.engine.game.System;
import helios.math;

import helios.core.units;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.engine.game.UpdateContext;

import helios.engine.game.components.physics.ScaleComponent;
import helios.engine.game.components.physics.TransformComponent;
import helios.engine.game.components.physics.AabbColliderComponent;

import helios.engine.game.components.model.ModelAabbComponent;

export namespace helios::engine::game::systems::physics {

    /**
     * @brief System that updates AABB colliders based on entity world transforms.
     *
     * @details
     * This system iterates through all GameObjects that have:
     * - ModelAabbComponent (source AABB)
     * - TransformComponent (world transform)
     * - AabbColliderComponent (target collider)
     *
     * When the TransformComponent is marked dirty, the system transforms the
     * model AABB into world space and updates the collider bounds.
     */
    class BoundsUpdateSystem : public System {

    public:

        /**
         * @brief Updates collider bounds for all applicable entities.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            const auto& gameObjects = gameWorld_->gameObjects();

            for (auto& gameObjectPair : gameObjects) {

                auto* obj = gameObjectPair.second.get();

                auto* mab = obj->get<helios::engine::game::components::model::ModelAabbComponent>();
                auto* tc = obj->get<helios::engine::game::components::physics::TransformComponent>();
                auto* bc = obj->get<helios::engine::game::components::physics::AabbColliderComponent>();

                if (bc && tc->isDirty()) {
                    bc->setBounds(mab->aabb().applyTransform(tc->worldTransform()));
                }

            }
        }

    };

};
