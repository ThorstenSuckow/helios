/**
 * @file bootstrap.ixx
 * @brief Engine bootstrap: component registration and GameWorld/GameLoop factory.
 */
module;

#include <utility>
#include <memory>

export module helios.engine.bootstrap;

import helios.engine.runtime.gameloop;
import helios.engine.runtime.world;

import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command;

import helios.engine.mechanics.lifecycle;
import helios.engine.mechanics.gamestate;
import helios.engine.mechanics.match;

import helios.engine.mechanics.registry;
import helios.engine.modules.registry;
import helios.engine.ecs.registry;



export namespace helios::engine::bootstrap {

    using namespace helios::engine::runtime::world;
    using namespace helios::engine::runtime::gameloop;

    /**
     * @brief Registers all component types with the ComponentReflector.
     *
     * @details This function must be called during engine initialization to
     * enable runtime reflection features such as cloning, lifecycle callbacks
     * (onAcquire, onRelease, onRemove), and enable/disable toggles.
     *
     * ## Usage
     *
     * ```cpp
     * // Call once during engine startup
     * helios::engine::bootstrap::registerAllComponents();
     * ```
     *
     * @note New component types must be added to the respective module's
     *       registry.ixx file to participate in the reflection system.
     *
     * @see ComponentReflector
     * @see ComponentOpsRegistry
     */
    inline void registerAllComponents() {

        static bool done = false;
        if (done) return;
        done = true;

        helios::engine::mechanics::registerComponents();
        helios::engine::modules::registerComponents();
        helios::engine::ecs::registerComponents();

    }

    /**
     * @brief Creates a pre-configured GameWorld and GameLoop pair.
     *
     * @details The factory heap-allocates both objects and performs the
     * minimal setup required before application-specific configuration:
     *
     * - Registers the `EngineCommandBuffer` with the GameWorld's
     *   ResourceRegistry.
     * - Tracks `GameState` and `MatchState` in the Session so that
     *   TypedPass state filtering works out of the box.
     *
     * The caller receives ownership via `unique_ptr` and is responsible
     * for registering Managers, configuring phases/passes, calling
     * `GameWorld::init()` and `GameLoop::init()`, and driving the
     * main loop.
     *
     * ## Usage
     *
     * ```cpp
     * helios::engine::bootstrap::registerAllComponents();
     *
     * auto [gameWorldPtr, gameLoopPtr] = helios::engine::bootstrap::makeGameWorld();
     * auto& gameWorld = *gameWorldPtr;
     * auto& gameLoop  = *gameLoopPtr;
     *
     * // Application-specific setup
     * gameWorld.registerResource<SpawnManager>();
     * gameLoop.phase(PhaseType::Pre)
     *     .addPass<GameState>(GameState::Any)
     *         .addSystem<InputSystem>();
     *
     * gameWorld.init();
     * gameLoop.init(gameWorld);
     * ```
     *
     * @return A pair of (GameWorld, GameLoop) unique pointers.
     *
     * @see registerAllComponents
     * @see GameWorld
     * @see GameLoop
     * @see EngineCommandBuffer
     * @see Session::trackState
     */
    inline std::pair<std::unique_ptr<GameWorld>, std::unique_ptr<GameLoop>> makeGameWorld() {
        auto gameLoop = std::make_unique<helios::engine::runtime::gameloop::GameLoop>();
        auto gameWorld = std::make_unique<helios::engine::runtime::world::GameWorld>();

        gameWorld->registerCommandBuffer<helios::engine::runtime::messaging::command::EngineCommandBuffer>();

        gameWorld->registerManager<helios::engine::mechanics::lifecycle::WorldLifecycleManager>();
        gameWorld->registerManager<helios::engine::mechanics::gamestate::GameStateManager>(
        helios::engine::mechanics::gamestate::rules::DefaultGameStateTransitionRules::rules());
        gameWorld->registerManager<helios::engine::mechanics::match::MatchStateManager>(
            helios::engine::mechanics::match::rules::DefaultMatchStateTransitionRules::rules());

        gameWorld->session().trackState<helios::engine::mechanics::gamestate::types::GameState>();
        gameWorld->session().trackState<helios::engine::mechanics::match::types::MatchState>();

        return std::make_pair(std::move(gameWorld), std::move(gameLoop));
    }

}

