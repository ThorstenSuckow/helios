# State Management

helios provides a generic, template-based state machine architecture for managing application and gameplay states. The system is fully parameterized by state type, enabling reuse for different state enums (e.g., GameState, MatchState).

## Overview

The state management system uses a common architectural pattern for all state types:

| Component | Description |
|-----------|-------------|
| `StateManager<StateType>` | Rule-based state machine with listener support |
| `StateTransitionRule<StateType>` | Defines valid transitions with optional guards |
| `StateTransitionListener<StateType>` | Interface for transition observers |
| `StateCommand<StateType>` | Command for requesting transitions |
| `StateComponent<StateType>` | Stores current state on entities |

### Example State Configurations

| State Type | Scope | Example States |
|------------|-------|----------------|
| `GameState` | Application lifecycle | Title, Running, Paused |
| `MatchState` | Match/round lifecycle | Warmup, PlayerSpawn, Playing |

## Hierarchical State Machine

helios supports hierarchical state management where lower-level states (e.g., MatchState) only apply within specific higher-level states (e.g., GameState::Running):

```
┌─────────────────────────────────────────────────────────────────────┐
│                         GameState                                   │
│                                                                     │
│  ┌──────────┐      StartRequested      ┌──────────────────────────┐ │
│  │  Title   │ ───────────────────────► │        Running           │ │
│  └──────────┘                          │                          │ │
│       ▲                                │  ┌────────────────────┐  │ │
│       │                                │  │    MatchState      │  │ │
│       │ ReturnToTitle                  │  │                    │  │ │
│       │                                │  │ ┌────────┐         │  │ │
│       │                                │  │ │Warmup  │         │  │ │
│       │                                │  │ └───┬────┘         │  │ │
│       │                                │  │     │ PlayerSpawn  │  │ │
│       │                                │  │     ▼              │  │ │
│       │                                │  │ ┌────────┐         │  │ │
│       │                                │  │ │Spawning│         │  │ │
│       │                                │  │ └───┬────┘         │  │ │
│       │                                │  │     │ StartMatch   │  │ │
│       │                                │  │     ▼              │  │ │
│       │                                │  │ ┌────────┐         │  │ │
│       │                                │  │ │Playing │         │  │ │
│       │                                │  │ └───┬────┘         │  │ │
│       │                                │  │     │ GameOver     │  │ │
│       │                                │  │     ▼              │  │ │
│       │                                │  │ ┌────────┐         │  │ │
│       └────────────────────────────────┼──┼─│GameOver│         │  │ │
│                                        │  │ └────────┘         │  │ │
│                                        │  └────────────────────┘  │ │
│                                        │                          │ │
│                                        │      TogglePause         │ │
│                                        └────────────┬─────────────┘ │
│                                                     │               │
│                                                     ▼               │
│                                        ┌────────────────────────┐   │
│                                        │        Paused          │   │
│                                        │  (MatchState frozen)   │   │
│                                        └────────────┬───────────┘   │
│                                                     │               │
│                                               TogglePause           │
│                                                     │               │
│                                                     ▼               │
│                                        ┌────────────────────────┐   │
│                                        │        Running         │   │
│                                        └────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────┘
```

### Key Concepts

- **GameState** is the top-level state controlling application lifecycle
- **MatchState** is a nested state that only progresses while `GameState::Running`
- When `GameState` transitions to `Paused`, the `MatchState` is frozen
- Transitions between states are triggered by `StateCommand<T>`

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                       Session                               │
│  ┌─────────────────────┐    ┌─────────────────────┐         │
│  │ StateComponent<GS>  │    │ StateComponent<MS>  │         │
│  └─────────────────────┘    └─────────────────────┘         │
└─────────────────────────────────────────────────────────────┘
         ▲                           ▲
         │                           │
┌────────┴──────────┐       ┌────────┴──────────┐
│ StateManager<GS>  │       │ StateManager<MS>  │
│  - rules[]        │       │  - rules[]        │
│  - listeners[]    │       │  - listeners[]    │
└───────────────────┘       └───────────────────┘
         ▲                           ▲
         │                           │
┌────────┴──────────┐       ┌────────┴──────────┐
│ StateCommand<GS>  │       │ StateCommand<MS>  │
└───────────────────┘       └───────────────────┘
```

## Session State Registration

Before using any state type with the state management system, it must be explicitly registered with the Session via `trackState<T>()`. This adds the corresponding `StateComponent<T>` to the session entity, enabling systems to read and write the state.

```cpp
auto& session = gameWorld.session();

// Register state types before use
session.trackState<GameState>();
session.trackState<MatchState>();
```

Explicit registration avoids hard-coded state types in the Session class. It enables custom state types without modifying engine code and ensures that StateComponent exists before StateManager attempts to update it. Furthermore, it makes state dependencies visible at initialization time.

## Type Trait Specialization

To use `StateManager<YourState>`, you must specialize `StateTransitionId`:

```cpp
// In your bindings module:
template<>
struct helios::engine::state::types::StateTransitionId<GameState> {
    using Type = GameStateTransitionId;
};

template<>
struct helios::engine::state::types::StateTransitionId<MatchState> {
    using Type = MatchStateTransitionId;
};
```

## Transition Flow

When a state transition is requested:

1. `StateCommand<T>` is submitted to the command buffer
2. `StateCommandDispatcher<T>` routes it to the registered `StateManager<T>`
3. During `flush()`, the manager processes pending commands
4. Matching rule is found and guard is evaluated (if present)
5. Listeners are notified in order:
   - `onStateExit(from)`
   - `onStateTransition(context)`
   - `onStateEnter(to)`
6. Session state is updated via `StateComponent<T>`

## Transition Rules

Rules define valid state changes with optional guards:

```cpp
using namespace helios::engine::state::types;

// Define rules as constexpr array
constexpr StateTransitionRule<GameState> gameStateRules[] = {
    // from, transitionId, to
    {GameState::Title, GameStateTransitionId::StartRequested, GameState::Running},
    {GameState::Running, GameStateTransitionId::TogglePause, GameState::Paused},
    {GameState::Paused, GameStateTransitionId::TogglePause, GameState::Running},
};
```

### Guarded Transitions

Guards are function pointers that can block a transition:

```cpp
// Guard callback type: bool(*)(UpdateContext&, const StateTransitionRequest<T>)
constexpr StateTransitionRule<GameState> guardedRule{
    GameState::Running,
    GameStateTransitionId::GameOver,
    GameState::Title,
    // Guard: only allow if player has no lives
    [](auto& ctx, auto& req) {
        return ctx.session().lives() == 0;
    }
};
```

## Listeners

Listeners observe state transitions and perform side effects:

### Interface

```cpp
template<typename StateType>
class StateTransitionListener {
public:
    virtual void onStateExit(UpdateContext& ctx, StateType from) = 0;
    virtual void onStateTransition(UpdateContext& ctx, StateTransitionContext<StateType> transitionCtx) = 0;
    virtual void onStateEnter(UpdateContext& ctx, StateType to) = 0;
};
```

### Lambda-based Listener

```cpp
using namespace helios::engine::state::listeners;

auto listener = std::make_unique<LambdaStateListener<GameState>>(
    // onExit
    [](auto& ctx, GameState from) {
        if (from == GameState::Running) {
            // Clean up running state
        }
    },
    // onTransition
    [](auto& ctx, auto transitionCtx) {
        // Log transition
    },
    // onEnter
    [](auto& ctx, GameState to) {
        if (to == GameState::Running) {
            ctx.gameWorld().reset();
        }
    }
);

stateManager->addStateListener(std::move(listener));
```

## Usage Example

### Creating State Managers

```cpp
using namespace helios::engine::state;

// Register state types with session (required before managers are flushed)
auto& session = gameWorld.session();
session.trackState<GameState>();
session.trackState<MatchState>();

// Create managers with rules
auto gameStateManager = std::make_unique<StateManager<GameState>>(gameStateRules);
auto matchStateManager = std::make_unique<StateManager<MatchState>>(matchStateRules);

// Add listeners
gameStateManager->addStateListener(std::make_unique<LambdaStateListener<GameState>>(
    /* callbacks */
));

// Register with GameLoop (managers are initialized and flushed automatically)
gameLoop.addManager(std::move(gameStateManager));
gameLoop.addManager(std::move(matchStateManager));
```

### Requesting State Transitions

```cpp
using namespace helios::engine::state::commands;
using namespace helios::engine::state::types;

// Request game start
updateContext.commandBuffer().add<StateCommand<GameState>>(
    StateTransitionRequest<GameState>{
        GameState::Title,
        GameStateTransitionId::StartRequested
    }
);

// Request player spawn in match
updateContext.commandBuffer().add<StateCommand<MatchState>>(
    StateTransitionRequest<MatchState>{
        MatchState::Warmup,
        MatchStateTransitionId::PlayerSpawnRequested
    }
);
```

### Querying Current State

```cpp
auto& session = gameWorld.session();

GameState gameState = session.state<GameState>();
MatchState matchState = session.state<MatchState>();

if (gameState == GameState::Running && matchState == MatchState::Playing) {
    // Game is actively playing
}
```

## State-to-ID Mapping

For associating viewports or menus with states, use the mapping utilities:

```cpp
using namespace helios::engine::state;

CombinedStateToIdMapPair<GameState, MatchState, ViewportId> viewportPolicy;

// Map state combinations to viewport IDs
viewportPolicy.add(GameState::Running, MatchState::Playing, ViewportId("game"));
viewportPolicy.add(GameState::Paused, MatchState::Undefined, ViewportId("pause_menu"));
viewportPolicy.add(GameState::Running, MatchState::GameOver, ViewportId("game_over"));

viewportPolicy.freeze();

// Query active viewports for current state
auto viewportIds = viewportPolicy.ids(gameState, matchState);
```

## Pass-Level State Filtering

Passes in the game loop can be configured to run only during specific states:

```cpp
// This pass only runs when game is Running
gameLoop.phase(PhaseType::Main)
    .addPass(GameState::Running)
        .addSystem<MovementSystem>()
        .addSystem<CollisionSystem>();

// This pass runs in all states
gameLoop.phase(PhaseType::Pre)
    .addPass(GameState::Any)
        .addSystem<InputSystem>();
```

## Related Topics

- [Game Loop Architecture](gameloop-architecture.md) - Phase/Pass structure
- [Command System](command-system.md) - Command-driven state changes
- [Event System](event-system.md) - Event propagation


