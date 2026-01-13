/**
 * @file ShootCommandDispatcher.ixx
 * @brief Dispatcher for routing ShootCommands to the ProjectilePoolManager.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.game.gameplay.combat.dispatcher.ShootCommandDispatcher;


import helios.engine.game.gameplay.combat.manager.ProjectilePoolManager;
import helios.engine.game.gameplay.combat.commands.ShootCommand;

import helios.engine.game.physics.motion.components.Move2DComponent;
import helios.engine.game.spatial.transform.components.TranslationStateComponent;
import helios.engine.game.gameplay.combat.components.Aim2DComponent;

import helios.engine.game.gameplay.combat.ProjectileSpawnRequest;
          

import helios.engine.core.messaging.command.TargetedCommandDispatcher;
import helios.engine.core.messaging.command.TypedTargetedCommandDispatcher;
import helios.engine.game.GameObject;

import helios.engine.game.GameWorld;


export namespace helios::engine::game::gameplay::combat::dispatcher {

    /**
     * @brief Dispatcher that routes ShootCommands to the ProjectilePoolManager.
     *
     * @details ShootCommandDispatcher is a typed command dispatcher that handles
     * ShootCommand instances targeting specific GameObjects. When a ShootCommand
     * is dispatched, the dispatcher:
     *
     * 1. Retrieves the source entity's position (TranslationStateComponent)
     * 2. Retrieves the aim direction (Aim2DComponent)
     * 3. Retrieves the source velocity for inheritance (Move2DComponent)
     * 4. Creates a ProjectileSpawnRequest with all gathered data
     * 5. Submits the request to the ProjectilePoolManager for deferred spawning
     *
     * This enables the command-based shooting architecture where input systems
     * emit ShootCommands that are later processed during the manager flush phase.
     *
     * @see ShootCommand
     * @see ProjectilePoolManager
     * @see ProjectileSpawnRequest
     */
    class ShootCommandDispatcher final : public helios::engine::core::messaging::command::TypedTargetedCommandDispatcher<
        helios::engine::game::gameplay::combat::commands::ShootCommand> {

        /**
         * @brief Reference to the ProjectilePoolManager for spawning projectiles.
         */
        helios::engine::game::gameplay::combat::manager::ProjectilePoolManager& projectilePoolManager_;

    protected:

        /**
         * @brief Dispatches a ShootCommand by creating a ProjectileSpawnRequest.
         *
         * @details Gathers position, aim direction, and velocity from the source
         * GameObject and submits a spawn request to the ProjectilePoolManager.
         *
         * @param gameObject The source GameObject that fired the shot.
         * @param command The ShootCommand containing intensity information.
         *
         * @pre gameObject must have Move2DComponent, TranslationStateComponent,
         *      and Aim2DComponent attached.
         */
        void dispatchTyped(
            helios::engine::game::GameObject& gameObject,
            const helios::engine::game::gameplay::combat::commands::ShootCommand& command
        ) noexcept override {

            auto* m2d = gameObject.get<helios::engine::game::physics::motion::components::Move2DComponent>();
            auto* t2c = gameObject.get<helios::engine::game::spatial::transform::components::TranslationStateComponent>();
            auto* a2c = gameObject.get<helios::engine::game::gameplay::combat::components::Aim2DComponent>();

            assert(m2d != nullptr && t2c != nullptr && a2c!= nullptr && "Unexpected nullptr when dispatching");

            const float throttle = 1.0f;

            helios::engine::game::gameplay::combat::ProjectileSpawnRequest shootRequest{
                t2c->translation(),
                a2c->direction().toVec3(),
                throttle,
                m2d != nullptr ? m2d->velocity() : helios::math::vec3f{0.0f, 0.0f, 0.0f},
                a2c->frequency(),

            };


            projectilePoolManager_.addRequest(shootRequest);
        };

    public:

        /**
         * @brief Constructs a ShootCommandDispatcher.
         *
         * @param projectilePoolManager Reference to the manager that handles
         *        projectile spawning. Must remain valid for the dispatcher's lifetime.
         */
        explicit ShootCommandDispatcher(
            helios::engine::game::gameplay::combat::manager::ProjectilePoolManager& projectilePoolManager
        ) :
        projectilePoolManager_(projectilePoolManager)
        {}




    };


}