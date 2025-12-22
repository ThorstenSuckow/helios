/**
 * @file ShootComponent.ixx
 * @brief Component for handling projectile shooting mechanics.
 */
module;

#include <cassert>
#include <iostream>
#include <memory>

export module helios.engine.game.components.physics.ShootComponent;

import helios.math.types;
import helios.math.utils;
import helios.engine.game.UpdateContext;
import helios.engine.game.Updatable;
import helios.engine.game.Component;
import helios.engine.game.components.gameplay.Aim2DComponent;
import helios.engine.game.components.scene.SceneNodeComponent;

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

import helios.engine.game.systems.BulletPool;

export namespace helios::engine::game::components::physics {

    /**
     * @brief Component for handling projectile shooting with rate limiting.
     *
     * @details Manages the shooting mechanics for a GameObject by coordinating with
     * aim-components for direction and a bullet pool for projectile spawning. Implements
     * a cooldown timer to control fire rate.
     *
     *
     * Example usage:
     * ```cpp
     * auto shootComponent = std::make_unique<ShootComponent>();
     * gameObject.add(std::move(shootComponent));
     *
     * // Trigger shooting (typically from ShootCommand)
     * shootComponent->shoot(1.0f);
     * ```
     */
    class ShootComponent : public helios::engine::game::Component, public helios::engine::game::Updatable {

    protected:

        /**
         * @brief Current fire intensity (0.0 to 1.0).
         *
         * @details Set by shoot() and cleared when intensity is zero.
         * Controls whether projectiles are spawned during update().
         */
        float intensity_ = 0.0f;

        /**
         * @brief Pointer to the aim component for direction.
         *
         * @details Set during onAttach(). Must not be null when shooting.
         */
        helios::engine::game::components::gameplay::Aim2DComponent* aimComponent_ = nullptr;

        /**
         * @brief Pointer to the scene node component for spawn position.
         *
         * @details Set during onAttach(). Must not be null when shooting.
         */
        helios::engine::game::components::scene::SceneNodeComponent* sceneNodeComponent_ = nullptr;

        /**
         * @brief Cooldown timer for fire rate limiting, in seconds.
         *
         * If cooldownDelta <= cooldownTime, the next bullet can be shot.
         *
         * @details Accumulates delta time between shots.
         */
        float cooldownDelta_ = 0.0f;

        /**
         * @brief Cooldown timer for fire rate limiting, in seconds.
         *
         * @details Accumulates delta time between shots.
         */
        float cooldownTime_ = 0.0625f;


        /**
         * @brief The bullet speed, in meters per second.
         */
        float bulletSpeed_ = 60.0f;


    public:

        /**
         * @brief Called when the component is attached to a GameObject.
         *
         * @details Retrieves references to required sibling components.
         *
         * @param gameObject The GameObject this component is being attached to.
         */
        void onAttach(::helios::engine::game::GameObject* gameObject) noexcept override {
            Component::onAttach(gameObject);

            aimComponent_ = gameObject->get<helios::engine::game::components::gameplay::Aim2DComponent>();
            sceneNodeComponent_ = gameObject->get<helios::engine::game::components::scene::SceneNodeComponent>();
            assert(aimComponent_ != nullptr && "Unexpected nullptr for aimComponent_");
            assert(sceneNodeComponent_ != nullptr && "Unexpected nullptr for sceneNodeComponent_");
        }

        /**
         * @brief Sets the shooting intensity.
         *
         * @details Call with a non-zero value to start shooting, or zero to stop.
         * The intensity affects whether projectiles are spawned during update().
         *
         * @param intensity Fire intensity (0.0 to 1.0). Zero stops firing.
         */
        void shoot(float intensity) {

            if (intensity == 0.0f || !aimComponent_) {
                intensity_ = 0.0f;
                return;
            }
            intensity_ = intensity;
        }

        /**
         * @brief Updates the component, spawning projectiles when ready.
         *
         * @details Checks the cooldown timer and aim direction. If conditions are met,
         * spawns a bullet from the BulletPool at the current position traveling in
         * the aim direction.
         *
         * @param updateContext The update context containing timing and game state.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            cooldownDelta_ += updateContext.deltaTime() * intensity_;

            if (intensity_ == 0.0f) {
                return;
            }

            const auto aimDir = aimComponent_->direction();

            if (cooldownDelta_ >= cooldownTime_ && aimDir.length() > helios::math::EPSILON_LENGTH) {
                cooldownDelta_ = 0;
                auto* bulletPool = updateContext.gameWorld()
                                        ->get<helios::engine::game::systems::BulletPool>();

                bulletPool->spawn(
                    sceneNodeComponent_->translation(),
                    aimDir.normalize().toVec3() * bulletSpeed_
                );
            }
        }


        /**
         * @brief Returns the current fire intensity.
         *
         * @return The intensity value (0.0 to 1.0).
         */
        [[nodiscard]] float intensity() const noexcept {
            return intensity_;
        }

        /**
         * @brief Returns the cooldown time between shots.
         *
         * @return Cooldown duration in seconds.
         */
        [[nodiscard]] float cooldownTime() const noexcept {
            return cooldownTime_;
        }

        /**
         * @brief Sets the cooldown time between shots.
         *
         * @param cooldown New cooldown duration in seconds.
         */
        void setCooldownTime(float cooldown) noexcept {
            cooldownTime_ = cooldown;
        }

        /**
         * @brief Returns the bullet speed.
         *
         * @return Bullet speed in meters per second.
         */
        [[nodiscard]] float bulletSpeed() const noexcept {
            return bulletSpeed_;
        }

        /**
         * @brief Sets the bullet speed.
         *
         * @param speed New bullet speed in meters per second.
         */
        void setBulletSpeed(float speed) noexcept {
            bulletSpeed_ = speed;
        }

    };


}

