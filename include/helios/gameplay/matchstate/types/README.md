# helios::gameplay::matchstate::types

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
- `Undefined`, `Warmup`, `Start`, `Countdown`, `Playing`, `PlayerDefeated`, `GameOver`

Supports bitwise operators for combining states.

## MatchStateTransitionId

Transition identifiers:
- `Undefined`, `WarmupRequest`, `StartRequest`, `CountdownRequest`, `PlayerSpawnRequest`, `PlayerDied`, `GameOverRequest`, `QuitRequest`

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
@namespace helios::gameplay::matchstate::types
@brief Core types for match state management.
@details Contains enums, structs, and classes for state machine operations.
</p></details>

