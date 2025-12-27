/**
 * @file ProjectilePoolSystem.ixx
 * @brief Object pool for efficient projectile management in game systems.
 */
module;

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>


export module helios.engine.game.systems.gameplay.ProjectilePoolSystem;


import helios.engine.game.System;
import helios.math;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.scene.SceneNode;

import helios.rendering;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.util.io.BasicStringFileReader;
import helios.rendering.model.config.MaterialProperties;

import helios.engine.game.components.gameplay.Aim2DComponent;
import helios.engine.game.components.scene.SceneNodeComponent;

import helios.engine.game.UpdateContext;
import helios.util.Guid;

export namespace helios::engine::game::systems::gameplay {

    /**
     * @brief Object pool system for efficient projectile spawning and lifecycle management.
     *
     * @details This system pre-allocates a fixed number of projectile objects to avoid runtime
     * allocations during gameplay. Projectiles are spawned from the pool and returned when
     * they are no longer active. Each projectile is represented as a `SceneNode` attached
     * to the scene graph root.
     *
     * The pool pattern ensures consistent performance in scenarios with high projectile
     * throughput (e.g., twin-stick shooters, projectile-hell games).
     *
     * @note To further reduce the number of draw calls, instancing should be considered.
     *
     * Example usage:
     * ```cpp
     * auto projectileRenderable = createProjectileRenderable();
     * auto pool = std::make_unique<ProjectilePoolSystem>(projectileRenderable, 100);
     * gameWorld.addSystem(std::move(pool));
     *
     * // During gameplay
     * pool->spawn(playerPosition, aimDirection * projectileSpeed);
     * ```
     */
    class ProjectilePoolSystem : public System {

    protected:

        /**
         * @brief Internal representation of an active projectile.
         */
        struct ProjectileObject {
            /**
             * @brief Pointer to the SceneNode representing this projectile.
             */
            helios::scene::SceneNode* projectileNodePtr = nullptr;

            /**
             * @brief Current muzzleVelocity vector of the projectile.
             */
            helios::math::vec3f muzzleVelocity{};

        };

        /**
         * @brief Collection of currently active projectiles in the scene.
         */
        std::vector<std::unique_ptr<ProjectileObject>> activeProjectiles_;

        /**
         * @brief Pool of inactive projectiles available for spawning.
         */
        std::vector<std::unique_ptr<ProjectileObject>> pool_;

        /**
         * @brief Shared renderable used for all projectile instances.
         */
        std::shared_ptr<helios::rendering::Renderable> projectileRenderable_;

        /**
         * @brief The aabb of the projectile, cached.
         */
        helios::math::aabbf projectileAABB_{};



        /**
         * @brief Current frequency factor (0.0 to 1.0) for fire rate control.
         */
        float intensity_ = 0.0f;

        /**
         * @brief Optional reference to an aim component for direction.
         */
        helios::engine::game::components::gameplay::Aim2DComponent* aimComponent_ = nullptr;

        /**
         * @brief Optional reference to a scene node component for spawn position.
         */
        helios::engine::game::components::scene::SceneNodeComponent* sceneNodeComponent_ = nullptr;

        /**
         * @brief Root node of the scene graph where projectiles are attached.
         */
        helios::scene::SceneNode* rootNode_ = nullptr;

        /**
         * @brief Maximum number of projectiles in the pool.
         */
        unsigned int poolSize_ = 0;

        /**
         * @brief Prototype for creating projectile render instances.
         */
        std::shared_ptr<helios::rendering::RenderPrototype> projectilePrototype_ = nullptr;

        bool projectilesInitialized_ = false;

        float sourceVelocityInheritFactor_ = 0.5f;

    public:

        /**
         * @brief Constructs a ProjectilePoolSystem with a shared renderable and pool size.
         *
         * @param projectileRenderable The renderable to use for all projectile instances.
         * @param poolSize Maximum number of projectiles to pre-allocate.
         * @param arenaBox The aabb in which the projectiles can spawn.
         */
        explicit ProjectilePoolSystem(
            std::shared_ptr<helios::rendering::Renderable> projectileRenderable,
            unsigned int poolSize
        ) :
        System(),
        projectileRenderable_(std::move(projectileRenderable)),
        poolSize_(poolSize) {

            pool_.reserve(poolSize);
            activeProjectiles_.reserve(poolSize);

            for (unsigned int i = 0; i < poolSize; i++) {
                auto projectile = std::make_unique<ProjectileObject>();
                pool_.emplace_back(std::move(projectile));
            }

        }

        /**
         * @brief Called when the system is added to a GameWorld.
         *
         * @details Initializes the scene root reference and creates SceneNodes
         * for all pooled projectiles, attaching them to the scene graph.
         *
         * @param gameWorld The GameWorld this system is added to.
         */
        void onAdd(helios::engine::game::GameWorld* gameWorld) noexcept override {
            System::onAdd(gameWorld);

            if (gameWorld_->hasLevel()) {
                // level might be not available here,
                // spawn will take care of initializing if necessary
                std::ignore = prepareProjectiles();
            }
        }

        [[nodiscard]] bool prepareProjectiles() {
            if (!gameWorld_->hasLevel()) {
                return false;
            }
            if (projectilesInitialized_) {
                return true;
            }
            rootNode_ = gameWorld_->level().rootNode();
            assert(rootNode_ != nullptr && "Unexpected nullptr for sceneRoot_");

            helios::scene::SceneNode* nodePtr = nullptr;

            for (auto& projectileObject : pool_) {
                auto node = std::make_unique<helios::scene::SceneNode>(projectileRenderable_);
                node->setActive(false);
                nodePtr = node.get();
                std::ignore = rootNode_->addNode(std::move(node));
                projectileObject->projectileNodePtr = nodePtr;
            }
            assert(nodePtr != nullptr && "Unexpected nullptr for nodePtr");
            projectileAABB_ = nodePtr->aabb();
            projectilesInitialized_ = true;
            return true;
        }

        /**
         * @brief Spawns a projectile from the pool at the given position with muzzleVelocity.
         *
         * @details If the pool is empty, the spawn request is silently ignored.
         * The projectile is moved from the inactive pool to the active projectiles list.
         *
         * @param position World-space spawn position.
         * @param muzzleVelocity Initial muzzleVelocity vector for the projectile.
         * @param aimDir The normalized aim direction vector.
         * @param sourceVelocity The velocity of the emitting object.
         */
        void spawn(
            helios::math::vec3f position,
            helios::math::vec3f muzzleVelocity,
            helios::math::vec3f aimDir,
            helios::math::vec3f sourceVelocity
        ) noexcept {

            if (pool_.empty()) {
                return;
            }

            if (!prepareProjectiles()) {
                return;
            }


            auto projectile = std::move(pool_.back());
            pool_.pop_back();

            projectile->muzzleVelocity = muzzleVelocity + sourceVelocity * sourceVelocityInheritFactor_;

            /**
             * @todo the ProjectilePoolSystem assumes to shoot in 2d, projected on the
             * xy plane. This has to be configured later on.
             */
            /**
             * we simply create a rotation matrix that rotates about the
             * z-axis:
             * cos A -sin A 0 0
             * sin A  cos A 0 0
             *     0      0 1 0
             *     0      0 0 1
             *
             * Since aimDir is normalized, we benefit from cos A = aimDir[0],
             * sin A = aimDir[1].
             */
            projectile->projectileNodePtr->setRotation(helios::math::mat4f{
                aimDir[0], aimDir[1], 0.0f, 0.0f,
                -aimDir[1], aimDir[0], 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            });

            projectile->projectileNodePtr->setTranslation(position);
            projectile->projectileNodePtr->setActive(true);

            activeProjectiles_.emplace_back(std::move(projectile));
        }

        /**
         * @brief Despawns the projectile at the specified index.
         *
         * @details Moves the projectile from the active list to the inactive pool
         * and deactivates its SceneNode so it is no longer rendered.
         *
         * @param idx Index of the projectile in the activeProjectiles_ vector.
         */
        void despawn(unsigned int idx) {

            assert(idx < activeProjectiles_.size() && "unexpected value for idx");

            activeProjectiles_[idx]->projectileNodePtr->setActive(false);
            auto ptr = std::move(activeProjectiles_[idx]);
            activeProjectiles_.erase(activeProjectiles_.begin() + idx);
            pool_.push_back(std::move(ptr));
        }

        /**
         * @brief Updates all active projectiles each frame.
         *
         * @details Moves each active projectile according to its muzzleVelocity scaled by delta time.
         *
         * @note update uses the cached projectileAABB_ to check whether the projectile should despawn.
         *
         * @param updateContext The update context containing frame timing information.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            for (unsigned int i = 0; i < activeProjectiles_.size();) { // i gets increment below

                auto& projectilePtr = activeProjectiles_[i];

                auto& projectile = *(projectilePtr->projectileNodePtr);

                auto translation = projectile.localTransform().translation() +
                                   projectilePtr->muzzleVelocity *
                                   updateContext.deltaTime();
                if (gameWorld_->level().bounds().contains(projectileAABB_ + translation)) {

                    projectile.setTranslation(translation);
                } else {
                    despawn(i);
                    // do not increment i since despawn() will erase the
                    // element and resize the vector
                    continue;
                }

                ++i;
            }

        }

    };

}
