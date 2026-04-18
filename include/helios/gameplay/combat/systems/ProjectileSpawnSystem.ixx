/**
 * @file ProjectileSpawnSystem.ixx
 * @brief System for spawning projectiles based on ShootComponent state.
 */
module;

#include <cassert>

export module helios.gameplay.combat.systems.ProjectileSpawnSystem;



import helios.state.Bindings;

import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;
import helios.gameplay.combat.components.ShootComponent;
import helios.gameplay.combat.components.Aim2DComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.gameplay.spawn.commands.SpawnCommand;
import helios.gameplay.spawn.types.SpawnContext;
import helios.gameplay.spawn.types.EmitterContext;
import helios.gameplay.spawn.types.SpawnProfileId;

import helios.math;

import helios.ecs.components.Active;

import helios.engine.common.tags.SystemRole;

using namespace helios::gameplay::spawn::types;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;
export namespace helios::gameplay::combat::systems {

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
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class ProjectileSpawnSystem {

        /**
         * @brief The spawn profile ID used for projectile creation.
         *
         * References a SpawnProfile in the spawn system that defines how
         * projectiles are placed and initialized.
         */
        const helios::gameplay::spawn::types::SpawnProfileId spawnProfileId_;


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Constructs a ProjectileSpawnSystem with the specified spawn profile.
         *
         * @param spawnProfileId The ID of the spawn profile to use for projectiles.
         */
        explicit ProjectileSpawnSystem(
            const helios::gameplay::spawn::types::SpawnProfileId& spawnProfileId
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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, tsc, ac, sc, active] : updateContext.view<
                THandle,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::gameplay::combat::components::Aim2DComponent<THandle>,
                helios::gameplay::combat::components::ShootComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {


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
                    updateContext.queueCommand<TCommandBuffer,
                        helios::gameplay::spawn::commands::SpawnCommand
                    >(
                        spawnProfileId_,
                        SpawnContext{
                            EmitterContext{
                                tsc->translation(),
                                sc->sourceVelocity() + (aimDirection * sc->projectileSpeed()),
                                entity.handle()
                            }
                        }
                    );
                }
            }
        }

    };


}