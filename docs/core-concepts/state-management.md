# Game and Match State Management

helios provides a rule-based state machine architecture for managing both **Game States** (application-level states like Title, Running, Paused) and **Match States** (gameplay-level states like Warmup, PlayerSpawn, Playing).

## Overview

The state management system consists of two parallel managers:

| Manager | Scope | Example States |
|---------|-------|----------------|
| `GameStateManager` | Application lifecycle | Title, Running, Paused |
| `MatchStateManager` | Match/round lifecycle | Warmup, PlayerSpawn, Playing |

Both managers share the same architectural pattern:

1. **Rule-based transitions** with optional guards
2. **Listener notifications** on state changes
3. **Command-driven** state changes via `CommandBuffer`
4. **Session integration** for centralized state storage

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                       Session                               │
│  ┌───────────────────┐      ┌───────────────────┐           │
│  │ GameStateComponent│      │MatchStateComponent│           │
│  └───────────────────┘      └───────────────────┘           │
└─────────────────────────────────────────────────────────────┘
         ▲                           ▲
         │                           │
┌────────┴────────┐         ┌────────┴────────┐
│ GameStateManager│         │MatchStateManager│
│  - rules[]      │         │  - rules[]      │
│  - listeners[]  │         │  - listeners[]  │
└─────────────────┘         └─────────────────┘
         ▲                           ▲
         │                           │
┌────────┴────────┐         ┌────────┴────────┐
│ GameStateCommand│         │MatchStateCommand│
└─────────────────┘         └─────────────────┘
```

## Game State Flow

The `GameStateManager` controls the overall application state:

```
┌─────────────┐   StartRequested   ┌─────────────┐
│    Title    │ ─────────────────► │   Running   │
└─────────────┘                    └─────────────┘
                                         │
                                   TogglePause
                                         │
                                         ▼
                                   ┌─────────────┐
                                   │   Paused    │
                                   └─────────────┘
                                         │
                                   TogglePause
                                         │
                                         ▼
                                   ┌─────────────┐
                                   │   Running   │
                                   └─────────────┘
```

### GameState Enum

| State | Description |
|-------|-------------|
| `Title` | Main menu / title screen |
| `Running` | Game is actively playing |
| `Paused` | Game is paused |

## Match State Flow

The `MatchStateManager` controls the match/round lifecycle within a running game:

```
┌───────────────┐   WarmupRequested   ┌───────────────┐
│   Undefined   │ ──────────────────► │    Warmup     │
└───────────────┘                     └───────────────┘
                                             │
                                    PlayerSpawnRequested
                                             │
                                             ▼
                                      ┌───────────────┐
                                      │  PlayerSpawn  │
                                      └───────────────┘
                                             │
                                       StartRequested
                                             │
                                             ▼
                                      ┌───────────────┐
                                      │    Playing    │
                                      └───────────────┘
```

### MatchState Enum

| State | Description |
|-------|-------------|
| `Undefined` | Initial state before match starts |
| `Warmup` | Pre-match preparation phase |
| `Intro` | Introduction/cutscene phase |
| `Countdown` | Countdown before gameplay |
| `PlayerSpawn` | Player entity spawning |
| `Playing` | Active gameplay |

## Transition Rules

Both managers use transition rules to define valid state changes:

```cpp
// GameStateTransitionRule(from, transitionId, to, type)
GameStateTransitionRule(
    GameState::Title, 
    GameStateTransitionId::StartRequested, 
    GameState::Running, 
    GameStateTransitionType::Standard
);
```

Rules can include guards for conditional transitions:

```cpp
GameStateTransitionRule rule{
    GameState::Running,
    GameStateTransitionId::TogglePause,
    GameState::Paused,
    GameStateTransitionType::Guarded,
    [](const UpdateContext& ctx) { return canPause(ctx); }
};
```

## Listeners

Listeners react to state changes and can perform actions like:
- Resetting the game world
- Spawning the player
- Showing/hiding UI elements
- Playing audio cues

```cpp
class WorldResetListener : public GameStateTransitionListener {
public:
    void onGameStateEnter(UpdateContext& ctx, GameState state) override {
        if (state == GameState::Running) {
            ctx.gameWorld().reset();
        }
    }
};

// Register with manager
gameStateManager->addGameStateListener(std::make_unique<WorldResetListener>());
```

## Usage Example

### Configuring State Managers

```cpp
// Create managers
auto gameStateManager = std::make_unique<GameStateManager>();
auto matchStateManager = std::make_unique<MatchStateManager>();

// Add listeners
gameStateManager->addGameStateListener(std::make_unique<WorldResetListener>());
matchStateManager->addMatchStateListener(std::make_unique<PlayerSpawnListener>(player));

// Register with GameWorld
gameWorld.addManager(std::move(gameStateManager));
gameWorld.addManager(std::move(matchStateManager));
```

### Requesting State Transitions

State transitions are requested via commands:

```cpp
// Request game start
commandBuffer.add<GameStateCommand>(
    GameStateTransitionRequest(GameState::Title, GameStateTransitionId::StartRequested)
);

// Request player spawn in match
commandBuffer.add<MatchStateCommand>(
    MatchStateTransitionRequest(MatchState::Warmup, MatchStateTransitionId::PlayerSpawnRequested)
);
```

### Querying Current State

Access the current state through the Session:

```cpp
auto& session = gameWorld.session();

GameState gameState = session.gameState();
MatchState matchState = session.matchState();

if (gameState == GameState::Running && matchState == MatchState::Playing) {
    // Game is actively playing
}
```

## Pass-Level State Filtering

Passes in the game loop can be configured to run only during specific game states:

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

- [Game Loop Architecture](gameloop-architecture.md) — Phase/Pass structure
- [Command System](command-system.md) — Command-driven state changes
- [Event System](event-system.md) — Event propagation


