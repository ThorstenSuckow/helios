/**
 * @file ShootComponent.ixx
 * @brief Component for handling projectile shooting mechanics.
 */
module;

#include <cassert>
#include <iostream>
#include <memory>

export module helios.engine.mechanics.combat.components.ShootComponent;

import helios.math.types;
import helios.math.utils;
import helios.engine.ecs.UpdateContext;
import helios.engine.ecs.Updatable;
import helios.engine.ecs.Component;
import helios.engine.mechanics.combat.components.Aim2DComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;

import helios.math;

import helios.engine.ecs.GameObject;
import helios.engine.ecs.GameWorld;
import helios.scene.SceneNode;

import helios.rendering;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.util.io.BasicStringFileReader;
import helios.rendering.model.config.MaterialProperties;

export namespace helios::engine::mechanics::combat::components {

    /**
     * @brief Component for handling projectile shooting with rate limiting.
     *
     * @details Manages the shooting mechanics for a GameObject by coordinating with
     * aim-components for direction and a projectile pool for projectile spawning. Implements
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
    class ShootComponent : public helios::engine::ecs::Component {

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
        helios::engine::mechanics::combat::components::Aim2DComponent* aimComponent_ = nullptr;

        /**
         * @brief Pointer to the scene node component for spawn position.
         *
         * @details Set during onAttach(). Must not be null when shooting.
         */
        helios::engine::game::spatial::transform::components::TransformComponent* transformComponent_ = nullptr;

        /**
         * @brief Cooldown timer for fire rate limiting, in seconds.
         *
         * If cooldownDelta <= cooldownTime, the next projectile can be shot.
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
         * @brief The projectile speed, in meters per second.
         */
        float projectileSpeed_ = 60.0f;

        /**
         * @brief Velocity of the source object in three-dimensional space.
         *
         * Used to adjust the trajectory of projectiles emitted by the source's movement.
         */
        helios::math::vec3f sourceVelocity_;


    public:

        /**
         * @brief Called when the component is attached to a GameObject.
         *
         * @details Retrieves references to required sibling components.
         *
         * @param gameObject The GameObject this component is being attached to.
         */
        void onAttach(::helios::engine::ecs::GameObject* gameObject) noexcept override {
            Component::onAttach(gameObject);

            aimComponent_ = gameObject->get<helios::engine::mechanics::combat::components::Aim2DComponent>();
            transformComponent_ = gameObject->get<helios::engine::game::spatial::transform::components::TransformComponent>();
            assert(aimComponent_ != nullptr && "Unexpected nullptr for aimComponent_");
            assert(transformComponent_ != nullptr && "Unexpected nullptr for transformComponent_");
        }

        /**
         * @brief Sets the shooting intensity.
         *
         * @details Call with a non-zero value to start shooting, or zero to stop.
         * The intensity affects whether projectiles are spawned during update().
         *
         * @param intensity Fire intensity (0.0 to 1.0). Zero stops firing.
         * @param sourceVelocity The velocity of the object emitting the projectile.
         */
        void shoot(float intensity, helios::math::vec3f sourceVelocity) {

            if (intensity == 0.0f || !aimComponent_) {
                intensity_ = 0.0f;
                return;
            }
            sourceVelocity_ = sourceVelocity;
            intensity_ = intensity;
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
         * @brief Returns the projectile speed.
         *
         * @return Projectile speed in meters per second.
         */
        [[nodiscard]] float projectileSpeed() const noexcept {
            return projectileSpeed_;
        }

        /**
         * @brief Sets the projectile speed.
         *
         * @param speed New projectile speed in meters per second.
         */
        void setProjectileSpeed(float speed) noexcept {
            projectileSpeed_ = speed;
        }

    };


}

