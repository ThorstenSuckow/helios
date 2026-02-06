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

import helios.math;

export namespace helios::engine::mechanics::combat::systems {

    /**
     * @brief System that spawns projectiles for entities with active ShootComponents.
     *
     * @details ProjectileSpawnSystem iterates all entities that have a ShootComponent,
     * Aim2DComponent, and TranslationStateComponent. When a ShootComponent has non-zero
     * intensity and the aim component has a valid frequency, the system calculates how
     * many projectiles to spawn based on accumulated time and cooldown.
     *
     * ## Fire Rate Mechanics
     *
     * The system implements an intensity-based fire rate:
     * 1. Accumulates `deltaTime * intensity` into the cooldown timer
     * 2. When timer >= cooldownDelta, spawns `floor(timer / cooldownDelta)` projectiles
     * 3. Reduces timer by spawned amount, preserving fractional remainder
     *
     * This allows continuous fire with variable intensity affecting the effective rate.
     *
     * ## Spawn Context
     *
     * Each SpawnCommand includes an EmitterContext containing:
     * - **position:** Current translation from TranslationStateComponent
     * - **velocity:** sourceVelocity + (aimDirection * projectileSpeed)
     *
     * Example setup:
     * ```cpp
     * auto projectileSpawnSystem = std::make_unique<ProjectileSpawnSystem>(
     *     SpawnProfileId{1}  // ID of the projectile spawn profile
     * );
     * mainPhase.addPass().add(std::move(projectileSpawnSystem));
     * ```
     *
     * @note Entities must have ShootComponent, Aim2DComponent, and
     *       TranslationStateComponent to be processed.
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
         *
         * 1. Skips if aim frequency or intensity are near zero
         * 2. Accumulates `deltaTime * intensity` into the cooldown timer
         * 3. If timer < cooldownDelta, updates timer and continues
         * 4. Calculates projectile count as `floor(timer / cooldownDelta)`
         * 5. Reduces timer by `count * cooldownDelta`
         * 6. Enqueues SpawnCommands with EmitterContext for each projectile
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, tsc, ac, sc] : gameWorld_->find<
                helios::engine::modules::spatial::transform::components::TranslationStateComponent,
                helios::engine::mechanics::combat::components::Aim2DComponent,
                helios::engine::mechanics::combat::components::ShootComponent
            >().each()) {


                const float intensity = sc->intensity();
                const float cooldown  = sc->cooldownDelta();

                float timer = sc->cooldownTimer();

                // only process for meaningful values
                assert(cooldown >  helios::math::EPSILON_LENGTH && "Unexpected cooldownDelta");
                if (ac->frequency() <=  helios::math::EPSILON_LENGTH ||
                    intensity <=  helios::math::EPSILON_LENGTH) {
                    continue;
                };

                const float delta = updateContext.deltaTime();

                // update the timer to accumulate frame times * intensity.
                // will be updated in this frame if projectiles spawned,
                timer += delta * intensity;

                if (timer < cooldown) {
                    sc->setCooldownTimer(timer);
                    continue;
                }

                const unsigned int amount = static_cast<unsigned int>(timer / cooldown);

                // reduce timer by the product of projectiles and cooldown,
                // which leaves a fractional part that did not add a whole projectile
                // to this frame
                sc->setCooldownTimer(timer - (amount * cooldown));

                const auto aimDirection = ac->direction().toVec3();
                assert(aimDirection.isNormalized() && "Unexpected aimDirection.length()");

                for (unsigned int i = 0; i < amount; i++) {
                    updateContext.commandBuffer().add<
                        helios::engine::runtime::spawn::commands::SpawnCommand
                    >(
                        spawnProfileId_,
                        helios::engine::runtime::spawn::SpawnContext{
                            helios::engine::runtime::spawn::EmitterContext{
                                tsc->translation(),
                                sc->sourceVelocity() + (aimDirection * sc->projectileSpeed()),
                                entity->entityHandle()
                            }
                        }
                    );
                }
            }
        }

    };


}