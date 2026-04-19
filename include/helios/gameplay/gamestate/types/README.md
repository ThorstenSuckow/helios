# helios::gameplay::gamestate::types

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
- `Undefined`, `Booted`, `Title`, `MatchReady`, `Running`, `Paused`, `Any`

Supports bitwise operators for combining states.

## GameStateTransitionId

Transition identifiers:
- `Undefined`, `BootRequest`, `ReadyMatchRequest`, `StartMatchRequest`, `TogglePause`, `TitleRequest`

## GameStateTransitionRule

Defines a valid transition with:
- Source state (`from`)
- Transition identifier (`transitionId`)
- Target state (`to`)
- Optional guard callback

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::gamestate::types
@brief Core types for game state management.
@details Contains enums, structs, and classes for state machine operations.
</p></details>
