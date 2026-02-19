# helios::engine::mechanics::match

Match state management for the helios engine.

This module provides domain-specific types and bindings for managing match states (Warmup, PlayerSpawn, Playing). It uses the generic `helios::engine::state` framework.

## Components

| Component | Description |
|-----------|-------------|
| `MatchStateManager` | Type alias for `StateManager<MatchState>` |
| `MatchState` | Enum defining match/round lifecycle states |
| `MatchStateTransitionId` | Enum defining valid transition triggers |

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `rules/` | Default transition rules and guards |
| `systems/` | Systems for match flow and rules |
| `types/` | Core types, enums, and type trait specializations |

## Architecture

`MatchStateManager` is a type alias for `StateManager<MatchState>`:

```cpp
using MatchStateManager = helios::engine::state::StateManager<types::MatchState>;
```

The generic `StateManager` provides:
- Rule-based transitions with optional guards
- Listener notifications on state changes
- Command-driven state changes via `StateCommand<MatchState>`

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
using namespace helios::engine::mechanics::match;
using namespace helios::engine::state;
using namespace helios::engine::state::commands;

// Create manager with rules
auto manager = std::make_unique<MatchStateManager>(matchStateRules);

// Add listeners
manager->addStateListener(std::make_unique<LambdaStateListener<types::MatchState>>(
    /* onExit, onTransition, onEnter callbacks */
));

// Submit transition via command
commandBuffer.add<StateCommand<types::MatchState>>(
    StateTransitionRequest<types::MatchState>{
        types::MatchState::Warmup,
        types::MatchStateTransitionId::PlayerSpawnRequested
    }
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::match
@brief Match state management for the helios engine.
@details Provides domain-specific types and bindings for managing match states using the generic helios::engine::state framework.
</p></details>

