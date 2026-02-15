# helios::engine::mechanics::gamestate::types

Core types for game state management.

## Types

| Type | Description |
|------|-------------|
| `GameState` | Enum of possible game states (bitmask) |
| `GameStateTransitionContext` | Context for a state transition |
| `GameStateTransitionId` | Enum of transition identifiers |
| `GameStateTransitionRequest` | Request to transition from a state |
| `GameStateTransitionRule` | Definition of a valid transition |
| `GameStateTransitionType` | Categorization of transitions |

## GameState

Bitmask enum with values:
- `Undefined`, `Loading`, `Title`, `Menu`, `Paused`, `Running`, `Any`

Supports bitwise operators for combining states.

## GameStateTransitionRule

Defines a valid transition with:
- Source state (`from`)
- Transition identifier (`transitionId`)
- Target state (`to`)
- Transition type
- Optional guard callback

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate::types
@brief Core types for game state management.
@details Contains enums, structs, and classes for state machine operations.
</p></details>

