# helios::engine::mechanics::gamestate

Game state management for the helios engine.

This module provides domain-specific types and bindings for managing game states (Title, Running, Paused). It uses the generic `helios::engine::state` framework.

## Components

| Component | Description |
|-----------|-------------|
| `GameStateManager` | Type alias for `StateManager<GameState>` |
| `GameState` | Enum defining application-level states |
| `GameStateTransitionId` | Enum defining valid transition triggers |

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `rules/` | Default transition rules and guards |
| `systems/` | Systems for input-driven state changes |
| `types/` | Core types, enums, and type trait specializations |

## Architecture

`GameStateManager` is a type alias for `StateManager<GameState>`:

```cpp
using GameStateManager = helios::engine::state::StateManager<types::GameState>;
```

The generic `StateManager` provides:
- Rule-based transitions with optional guards
- Listener notifications on state changes
- Command-driven state changes via `StateCommand<GameState>`

## State Flow

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
```

## Usage

```cpp
using namespace helios::engine::mechanics::gamestate;
using namespace helios::engine::state;
using namespace helios::engine::state::commands;

// Create manager with rules
auto manager = std::make_unique<GameStateManager>(gameStateRules);

// Add listeners
manager->addStateListener(std::make_unique<LambdaStateListener<types::GameState>>(
    /* onExit, onTransition, onEnter callbacks */
));

// Submit transition via command
commandBuffer.add<StateCommand<types::GameState>>(
    StateTransitionRequest<types::GameState>{
        types::GameState::Title,
        types::GameStateTransitionId::StartRequested
    }
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate
@brief Game state management for the helios engine.
@details Provides domain-specific types and bindings for managing game states using the generic helios::engine::state framework.
</p></details>

