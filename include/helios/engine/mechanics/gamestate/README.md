# helios::engine::mechanics::gamestate

Game state management system for the helios engine.

This module provides a rule-based state machine for managing game states (e.g., Title, Running, Paused) with transitions, listeners, and command-driven state changes.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `commands/` | Commands for requesting state transitions |
| `components/` | Components for storing game state |
| `dispatcher/` | Dispatchers for routing state commands |
| `listeners/` | Listeners for reacting to state transitions |
| `rules/` | Transition rules defining valid state changes |
| `systems/` | Systems for input-driven state changes |
| `types/` | Core types, enums, and data structures |

## Architecture

1. **GameStateManager** holds the transition rules and processes state commands
2. **GameStateCommand** encapsulates a transition request
3. **GameStateTransitionRule** defines valid from -> to transitions with optional guards
4. **GameStateTransitionListener** implementations react to state changes

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
// Create manager with default rules
auto manager = std::make_unique<GameStateManager>();

// Add listeners
manager->addGameStateListener(std::make_unique<WorldResetListener>());
manager->addGameStateListener(std::make_unique<FocusMenuListener>(menuId));

// Submit transition via command
commandBuffer.add<GameStateCommand>(
    GameStateTransitionRequest(GameState::Title, GameStateTransitionId::StartRequested)
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate
@brief Game state management system for the helios engine.
@details Provides a rule-based state machine for managing game states with transitions, listeners, and command-driven state changes.
</p></details>

