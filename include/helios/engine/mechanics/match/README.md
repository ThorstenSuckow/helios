# helios::engine::mechanics::match

Match state management system for the helios engine.

This module provides a rule-based state machine for managing match states (e.g., Warmup, PlayerSpawn, Playing) with transitions, listeners, and command-driven state changes.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `commands/` | Commands for requesting state transitions |
| `components/` | Components for storing match state and timer |
| `dispatcher/` | Dispatchers for routing state commands |
| `listeners/` | Listeners for reacting to state transitions |
| `systems/` | Systems for match flow and rules |
| `types/` | Core types, enums, and data structures |

## Architecture

1. **MatchStateManager** holds the transition rules and processes state commands
2. **MatchStateCommand** encapsulates a transition request
3. **MatchStateTransitionRule** defines valid from -> to transitions with optional guards
4. **MatchStateTransitionListener** implementations react to state changes

## Match Flow

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

## Usage

```cpp
// Create manager
auto manager = std::make_unique<MatchStateManager>();

// Add listeners
manager->addMatchStateListener(std::make_unique<PlayerSpawnListener>(playerObject));

// Submit transition via command
commandBuffer.add<MatchStateCommand>(
    MatchStateTransitionRequest(MatchState::Warmup, MatchStateTransitionId::PlayerSpawnRequested)
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::match
@brief Match state management system for the helios engine.
@details Provides a rule-based state machine for managing match states with transitions, listeners, and command-driven state changes.
</p></details>

