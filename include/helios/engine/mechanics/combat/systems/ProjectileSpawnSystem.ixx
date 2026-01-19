/**
 * @file ProjectileSpawnSystem.ixx
 * @brief System for spawning projectiles based on ShootComponent state.
 */
module;

#include <helios/engine/ecs/query/GameObjectView.h>

export module helios.engine.mechanics.combat.systems.ProjectileSpawnSystem;

import helios.engine.ecs.System;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.mechanics.combat.components.ShootComponent;
import helios.engine.mechanics.combat.components.Aim2DComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.runtime.spawn.commands.SpawnCommand;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.EmitterContext;
import helios.engine.core.data.SpawnProfileId;

export namespace helios::engine::mechanics::combat::systems {

    /**
     * @brief System that spawns projectiles for entities with active ShootComponents.
     *
     * @details ProjectileSpawnSystem iterates all entities that have a ShootComponent,
     * Aim2DComponent, and TranslationStateComponent. When a ShootComponent has non-zero
     * intensity and a valid aim direction, the system creates a SpawnCommand to spawn
     * a projectile from the configured spawn profile.
     *
     * The system uses the EmitterContext to pass the emitter's position and
     * the calculated projectile velocity (source velocity + aim direction * speed)
     * to the spawn logic.
     *
     * Example setup:
     * ```cpp
     * // Create system with projectile spawn profile
     * auto projectileSpawnSystem = std::make_unique<ProjectileSpawnSystem>(
     *     SpawnProfileId{1}  // ID of the projectile spawn profile
     * );
     * ```
     *
     * @note Entities must have all three required components (ShootComponent,
     *       Aim2DComponent, TranslationStateComponent) to be processed.
     *
     * @see ShootComponent
     * @see Aim2DComponent
     * @see SpawnCommand
     * @see EmitterContext
     */
    class ProjectileSpawnSystem : public helios::engine::ecs::System {

        /**
         * @brief The spawn profile ID used for projectile creation.
         *
         * References a SpawnProfile in the spawn system that defines how
         * projectiles are placed and initialized.
         */
        const helios::engine::core::data::SpawnProfileId spawnProfileId_;

    public:

        /**
         * @brief Constructs a ProjectileSpawnSystem with the specified spawn profile.
         *
         * @param spawnProfileId The ID of the spawn profile to use for projectiles.
         */
        explicit ProjectileSpawnSystem(
            const helios::engine::core::data::SpawnProfileId& spawnProfileId
        ) :
            spawnProfileId_(spawnProfileId)
        {}

        /**
         * @brief Processes all shooting entities and spawns projectiles.
         *
         * @details For each entity with ShootComponent, Aim2DComponent, and
         * TranslationStateComponent:
         * 1. Checks if intensity > 0 and direction is valid (normalized)
         * 2. Calculates projectile velocity from source velocity and aim direction
         * 3. Enqueues a SpawnCommand with EmitterContext containing position and velocity
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, tsc, ac, sc] : gameWorld_->find<
                helios::engine::modules::spatial::transform::components::TranslationStateComponent,
                helios::engine::mechanics::combat::components::Aim2DComponent,
                helios::engine::mechanics::combat::components::ShootComponent
            >().each()) {

                float intensity = sc->intensity();
                auto direction = ac->direction();

                if (intensity == 0.0f || !direction.isNormalized()) {
                    continue;
                }

                //float cooldown  = sc->cooldownTime();
                auto sourceVelocity = sc->sourceVelocity();
                auto projectileSpeed = sc->projectileSpeed();

                updateContext.commandBuffer().add<
                    helios::engine::runtime::spawn::commands::SpawnCommand
                >(
                    spawnProfileId_,
                    helios::engine::runtime::spawn::SpawnContext{
                        helios::engine::runtime::spawn::EmitterContext{
                            tsc->translation(),
                            sourceVelocity + (direction.toVec3() * projectileSpeed)
                        }
                    }
                );
            }
        }

    };


}