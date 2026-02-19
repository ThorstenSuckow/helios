# helios::engine::state::types

Type definitions for state transitions.

## Components

| Type | Description |
|------|-------------|
| `StateTransitionId<StateType>` | Type trait mapping state to transition ID type |
| `StateTransitionType<StateType>` | Type trait for transition customization |
| `StateTransitionContext<StateType>` | Complete transition context (from, to, id) |
| `StateTransitionRequest<StateType>` | Request to perform a transition |
| `StateTransitionRule<StateType>` | Defines a valid transition with optional guard |
| `GuardCallback<StateType>` | Function pointer type for transition guards |

## Type Trait Specialization

`StateTransitionId` must be specialized for each state type:

```cpp
template<>
struct StateTransitionId<GameState> {
    using Type = GameStateTransitionId;
};
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state::types
@brief Type definitions for state transitions.
@details Provides type traits, context classes, and rule definitions used by the state management system.
</p></details>

