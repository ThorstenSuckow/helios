# helios::engine::mechanics::match::types

Core types for match state management.

## Types

| Type | Description |
|------|-------------|
| `MatchState` | Enum of possible match states (bitmask) |
| `MatchStateTransitionContext` | Context for a state transition |
| `MatchStateTransitionId` | Enum of transition identifiers |
| `MatchStateTransitionRequest` | Request to transition from a state |
| `MatchStateTransitionRule` | Definition of a valid transition |
| `MatchStateTransitionType` | Categorization of transitions |

## MatchState

Bitmask enum with values:
- `Undefined`, `Warmup`, `Intro`, `Countdown`, `PlayerSpawn`, `Playing`, `PlayerDeath`, `Any`

Supports bitwise operators for combining states.

## MatchStateTransitionRule

Defines a valid transition with:
- Source state (`from`)
- Transition identifier (`transitionId`)
- Target state (`to`)
- Transition type
- Optional guard callback

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::match::types
@brief Core types for match state management.
@details Contains enums, structs, and classes for state machine operations.
</p></details>

