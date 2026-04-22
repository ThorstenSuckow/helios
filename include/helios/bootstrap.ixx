/**
 * @file bootstrap.ixx
 * @brief Engine bootstrap: component registration and GameWorld/GameLoop factory.
 */
module;

#include <cassert>
#include <cstddef>
#include <helios/helios_config.h>
#include <memory>
#include <utility>

export module helios.bootstrap;

import helios.runtime.gameloop;
import helios.runtime.world;

import helios.state.Bindings;
import helios.state.types.StateTransitionContext;
import helios.runtime.messaging.command;

import helios.platform;

import helios.registry;
import helios.scene.registry;
import helios.spatial.registry;
import helios.ui.registry;
import helios.scene.registry;
import helios.physics.registry;
import helios.gameplay.registry;
import helios.ecs.registry;
import helios.rendering.registry;
import helios.runtime.registry;

import helios.gameplay.lifecycle;
import helios.runtime.timing;
import helios.gameplay.gamestate;
import helios.gameplay.matchstate;

using namespace helios::state::types;
using namespace helios::gameplay::gamestate::types;
using namespace helios::platform::environment;
using namespace helios::platform::window;
using namespace helios::runtime::world;
using namespace helios::runtime::gameloop;
using namespace helios::runtime::messaging::command;

export namespace helios::bootstrap {

    template<typename... Tuple>
    struct ComponentRegistrar;

    template<typename... TEntityManagers>
    struct ComponentRegistrar<std::tuple<TEntityManagers...>> {
        static void registerComponents() {
            (helios::registerComponents<TEntityManagers>(), ...);
            (helios::gameplay::registerComponents<TEntityManagers>(), ...);
            (helios::ecs::registerComponents<TEntityManagers>(), ...);
            (helios::rendering::registerComponents<TEntityManagers>(), ...);
            (helios::runtime::registerComponents<TEntityManagers>(), ...);
            (helios::scene::registerComponents<TEntityManagers>(), ...);
            (helios::spatial::registerComponents<TEntityManagers>(), ...);
            (helios::ui::registerComponents<TEntityManagers>(), ...);
            (helios::physics::registerComponents<TEntityManagers>(), ...);
        }

    };


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
        if (done) {
            return;
        }
        done = true;

        ComponentRegistrar<RegisteredEntityManagers>::registerComponents();

    }

    /**
     * @brief Creates a pre-configured GameWorld and GameLoop pair.
     *
     * @details The factory heap-allocates both objects and performs the
     * minimal setup required before application-specific configuration:
     *
     * - Calls `registerAllComponents()` via `ComponentRegistrar`
     * - Registers `WorldLifecycleManager`, `GameStateManager`,
     *   `MatchStateManager`, and `TimerManager`
     * - Tracks `GameState` and `MatchState` in the Session
     * - Registers `RenderCommandBuffer`, `EngineCommandBuffer`,
     *   and `StateCommandBuffer`
     * - Sets initial `GameState` to `Booting`
     *
     * The caller receives ownership via `unique_ptr` and is responsible
     * for registering Managers, configuring phases/passes, calling
     * `GameWorld::init()` and `GameLoop::init()`, and driving the
     * main loop.
     *
     * ## Usage
     *
     * ```cpp
     * auto [gameWorldPtr, gameLoopPtr] = helios::engine::bootstrap::bootstrapGameWorld();
     * auto& gameWorld = *gameWorldPtr;
     * auto& gameLoop  = *gameLoopPtr;
     *
     * // Application-specific setup
     * gameWorld.registerManager<SpawnManager<GameObjectHandle>>();
     * gameLoop.phase(PhaseType::Pre)
     *     .addPass<GameState>(GameState::Any)
     *         .addSystem<InputSystem>();
     *
     * gameWorld.init();
     * gameLoop.init(gameWorld);
     * ```
     *
     * @note `registerAllComponents()` is called automatically — no separate
     *       call is required.
     *
     * @param capacity Initial capacity for the EntityManager's SparseSets.
     *                 Must be large enough to accommodate all entities including
     *                 pooled clones. Defaults to ENTITY_MANAGER_DEFAULT_CAPACITY.
     *
     * @return A pair of (GameWorld, GameLoop) unique pointers.
     *
     * @see registerAllComponents
     * @see GameWorld
     * @see GameLoop
     * @see EngineCommandBuffer
     * @see Session::trackState
     */
    inline std::pair<std::unique_ptr<GameWorld>, std::unique_ptr<GameLoop>> bootstrapGameWorld(
        const size_t capacity = ENTITY_MANAGER_DEFAULT_CAPACITY
    ) {
        auto gameWorld = std::make_unique<helios::runtime::world::GameWorld>(capacity);

        auto gameLoop = std::make_unique<helios::runtime::gameloop::GameLoop>(*gameWorld);


        registerAllComponents();

        gameWorld->registerManager<helios::gameplay::lifecycle::WorldLifecycleManager>();
        gameWorld->registerManager<helios::gameplay::gamestate::GameStateManager>(
        helios::gameplay::gamestate::rules::DefaultGameStateTransitionRules::rules());

        gameWorld->registerManager<helios::gameplay::matchstate::MatchStateManager>(
            helios::gameplay::matchstate::rules::DefaultMatchStateTransitionRules::rules());
        gameWorld->registerManager<helios::runtime::timing::TimerManager>();

        gameWorld->session().trackState<helios::gameplay::gamestate::types::GameState>();
        gameWorld->session().trackState<helios::gameplay::matchstate::types::MatchState>();

        gameWorld->registerCommandBuffer<RenderCommandBuffer>();
        gameWorld->registerCommandBuffer<PlatformCommandBuffer>();
        gameWorld->registerCommandBuffer<EngineCommandBuffer>();
        gameWorld->registerCommandBuffer<StateCommandBuffer>();


        gameWorld->session().setStateFrom<GameState>(
            StateTransitionContext<GameState>(
            GameState::Undefined,
            GameState::Booting,
            GameStateTransitionId::BootRequest
        ));


        return std::make_pair(std::move(gameWorld), std::move(gameLoop));
    }

}

