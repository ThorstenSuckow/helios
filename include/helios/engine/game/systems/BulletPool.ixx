/**
 * @file BulletPool.ixx
 * @brief Object pool for efficient bullet management in game systems.
 */
module;

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>


export module helios.engine.game.systems.BulletPool;


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

export namespace helios::engine::game::systems {

    /**
     * @brief Object pool system for efficient bullet spawning and lifecycle management.
     *
     * @details This system pre-allocates a fixed number of bullet objects to avoid runtime
     * allocations during gameplay. Bullets are spawned from the pool and returned when
     * they are no longer active. Each bullet is represented as a `SceneNode` attached
     * to the scene graph root.
     *
     * The pool pattern ensures consistent performance in scenarios with high bullet
     * throughput (e.g., twin-stick shooters, bullet-hell games).
     *
     * @note To further reduce the number of draw calls, instancing should be considered.
     *
     * Example usage:
     * ```cpp
     * auto bulletRenderable = createBulletRenderable();
     * auto pool = std::make_unique<BulletPool>(bulletRenderable, 100);
     * gameWorld.addSystem(std::move(pool));
     *
     * // During gameplay
     * pool->spawn(playerPosition, aimDirection * bulletSpeed);
     * ```
     */
    class BulletPool : public System {

    protected:

        /**
         * @brief Internal representation of an active bullet.
         */
        struct BulletObject {
            /**
             * @brief Pointer to the SceneNode representing this bullet.
             */
            helios::scene::SceneNode* bulletNodePtr = nullptr;

            /**
             * @brief Current velocity vector of the bullet.
             */
            helios::math::vec3f velocity{};
        };

        /**
         * @brief Collection of currently active bullets in the scene.
         */
        std::vector<std::unique_ptr<BulletObject>> activeBullets_;

        /**
         * @brief Pool of inactive bullets available for spawning.
         */
        std::vector<std::unique_ptr<BulletObject>> pool_;

        /**
         * @brief Shared renderable used for all bullet instances.
         */
        std::shared_ptr<helios::rendering::Renderable> bulletRenderable_;

        /**
         * @brief The aabb of the bullet, cached.
         */
        helios::math::aabbf bulletAABB_{};



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
         * @brief Root node of the scene graph where bullets are attached.
         */
        helios::scene::SceneNode* sceneRoot_ = nullptr;

        /**
         * @brief Maximum number of bullets in the pool.
         */
        unsigned int poolSize_ = 0;

        /**
         * @brief Prototype for creating bullet render instances.
         */
        std::shared_ptr<helios::rendering::RenderPrototype> bulletPrototype_ = nullptr;

        /**
         * @brief The aabb for the arena, in which bounds bullets can spawn.
         *
         * If any bullet outside the box is found, it will automatically despawn.
         *
         * @see update()
         *
         * @todo should be implemented using an interface "ArenaProvider" or similar
         */
        helios::math::aabbf arenaBox_{};

    public:

        /**
         * @brief Constructs a BulletPool with a shared renderable and pool size.
         *
         * @param bulletRenderable The renderable to use for all bullet instances.
         * @param poolSize Maximum number of bullets to pre-allocate.
         * @param arenaBox The aabb in which the bullets can spawn.
         */
        explicit BulletPool(
            std::shared_ptr<helios::rendering::Renderable> bulletRenderable,
            unsigned int poolSize,
            const helios::math::aabbf& arenaBox
        ) :
        System(),
        bulletRenderable_(std::move(bulletRenderable)),
        poolSize_(poolSize),
        arenaBox_(arenaBox) {

            pool_.reserve(poolSize);
            activeBullets_.reserve(poolSize);

            for (int i = 0; i < poolSize; i++) {
                auto bullet = std::make_unique<BulletObject>();
                pool_.emplace_back(std::move(bullet));
            }

        }

        /**
         * @brief Called when the system is added to a GameWorld.
         *
         * @details Initializes the scene root reference and creates SceneNodes
         * for all pooled bullets, attaching them to the scene graph.
         *
         * @param gameWorld The GameWorld this system is added to.
         */
        void onAdd(::helios::engine::game::GameWorld* gameWorld) noexcept override {
            System::onAdd(gameWorld);
            sceneRoot_ = &(gameWorld_->scene()->root());
            assert(sceneRoot_ != nullptr && "Unexpected nullptr for sceneRoot_");

            helios::scene::SceneNode* nodePtr = nullptr;

            for (auto& bulletObject : pool_) {
                auto node = std::make_unique<helios::scene::SceneNode>(bulletRenderable_);
                node->setActive(false);
                nodePtr = node.get();
                std::ignore = sceneRoot_->addNode(std::move(node));
                bulletObject->bulletNodePtr = nodePtr;
            }
            assert(nodePtr != nullptr && "Unexpected nullptr for nodePtr");
            bulletAABB_ = nodePtr->aabb();
        }

        /**
         * @brief Spawns a bullet from the pool at the given position with velocity.
         *
         * @details If the pool is empty, the spawn request is silently ignored.
         * The bullet is moved from the inactive pool to the active bullets list.
         *
         * @param position World-space spawn position.
         * @param velocity Initial velocity vector for the bullet.
         */
        void spawn(helios::math::vec3f position, helios::math::vec3f velocity) noexcept {

            if (pool_.empty()) {
                return;
            }

            auto bullet = std::move(pool_.back());
            pool_.pop_back();
            bullet->velocity = velocity;
            bullet->bulletNodePtr->setTranslation(position);
            bullet->bulletNodePtr->setActive(true);

            activeBullets_.emplace_back(std::move(bullet));
        }

        /**
         * @brief Despawns the bullet at the specified index.
         *
         * @details Moves the bullet from the active list to the inactive pool
         * and deactivates its SceneNode so it is no longer rendered.
         *
         * @param idx Index of the bullet in the activeBullets_ vector.
         */
        void despawn(unsigned int idx) {

            assert(idx < activeBullets_.size() && "unexpected value for idx");

            activeBullets_[idx]->bulletNodePtr->setActive(false);
            auto ptr = std::move(activeBullets_[idx]);
            activeBullets_.erase(activeBullets_.begin() + idx);
            pool_.push_back(std::move(ptr));
        }

        /**
         * @brief Updates all active bullets each frame.
         *
         * @details Moves each active bullet according to its velocity scaled by delta time.
         *
         * @note update uses the cached bulletAABB_ to check whether the bullet should despawn.
         *
         * @param updateContext The update context containing frame timing information.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            for (unsigned int i = 0; i < activeBullets_.size(); /* i gets increment below */ ) {

                auto& bulletPtr = activeBullets_[i];

                auto& bullet = *(bulletPtr->bulletNodePtr);

                auto translation = bullet.localTransform().translation() +
                                   bulletPtr->velocity *
                                   updateContext.deltaTime();
                if (arenaBox_.contains(bulletAABB_ + translation)) {
                    bullet.setTranslation(translation);
                } else {
                    despawn(i);
                    // do not increment i since despawn() will erase the
                    // element ad resize the vector
                    continue;
                }

                ++i;
            }


        }

    };

};
