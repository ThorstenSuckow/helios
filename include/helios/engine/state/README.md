# helios::engine::state

Generic, template-based state management system with rule-based transitions.

## Overview

This module provides a complete state management framework including:

- **State machines** with rule-based transitions and guards
- **Commands and dispatchers** for requesting state changes
- **Listeners** for reacting to state transitions
- **ID mapping utilities** for associating states with viewports/menus

All components are parameterized by state type, enabling reuse for different state enums (e.g., GameState, MatchState).

## Core Components

### State Management

| Class | Description |
|-------|-------------|
| `StateManager<StateType>` | Rule-based state machine with listener support |
| `StateTransitionRule<StateType>` | Defines valid transitions with optional guards |
| `StateTransitionListener<StateType>` | Interface for transition observers |
| `LambdaStateListener<StateType>` | Lambda-based listener implementation |
| `StateCommand<StateType>` | Command for requesting transitions |
| `StateCommandDispatcher<StateType>` | Routes commands to handlers |
| `StateComponent<StateType>` | Stores current state on entities |

### ID Mapping

| Class | Description |
|-------|-------------|
| `StateToIdMap<TState, TId>` | Maps a single state type to ID lists |
| `StateToIdMapPair<LState, RState, TId>` | Combines two state maps, returns union |
| `CombinedStateToIdMapPair<LState, RState, TId>` | Maps state pairs directly |

## Type Trait Specialization

To use `StateManager<YourState>`, you must specialize `StateTransitionId`:

```cpp
// In your bindings module:
template<>
struct helios::engine::state::types::StateTransitionId<GameState> {
    using Type = GameStateTransitionId;
};
```

## Usage

### Defining Rules

```cpp
using namespace helios::engine::state;
using namespace helios::engine::state::types;

constexpr StateTransitionRule<GameState> gameStateRules[] = {
    {GameState::MainMenu, GameStateTransitionId::StartGame, GameState::Running},
    {GameState::Running, GameStateTransitionId::Pause, GameState::Paused},
    {GameState::Paused, GameStateTransitionId::Resume, GameState::Running},
    {GameState::Running, GameStateTransitionId::GameOver, GameState::MainMenu,
        // Optional guard:
        [](auto& ctx, auto& req) { return ctx.session().lives() == 0; }
    }
};
```

### Creating the Manager

```cpp
auto gameStateManager = std::make_unique<StateManager<GameState>>(gameStateRules);

gameStateManager->addStateListener(
    std::make_unique<LambdaStateListener<GameState>>(
        [](auto& ctx, GameState from) { /* onExit */ },
        [](auto& ctx, auto transitionCtx) { /* onTransition */ },
        [](auto& ctx, GameState to) { /* onEnter */ }
    )
);

gameLoop.addManager(std::move(gameStateManager));
```

### Requesting Transitions

```cpp
updateContext.commandBuffer().add<StateCommand<GameState>>(
    StateTransitionRequest<GameState>{
        GameState::Running,
        GameStateTransitionId::Pause
    }
);
```

## Transition Flow

1. `StateCommand` is submitted to the command buffer
2. `StateCommandDispatcher` routes it to `StateManager`
3. `StateManager::flush()` processes pending commands
4. Matching rule is found and guard is evaluated
5. Listeners are notified: `onStateExit` → `onStateTransition` → `onStateEnter`
6. Session state is updated

## ID Mapping

For viewport/menu activation based on state:

```cpp
CombinedStateToIdMapPair<GameState, MatchState, ViewportId> viewportPolicy;

viewportPolicy.add(GameState::Running, MatchState::Playing, ViewportId("game"));
viewportPolicy.add(GameState::Paused, MatchState::Undefined, ViewportId("pause"));

viewportPolicy.freeze();
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state
@brief Generic, template-based state management system.
@details Provides a complete framework for managing application state with rule-based transitions, guards, listeners, and ID mapping utilities. All components are parameterized by state type for maximum reuse.
</p></details>

