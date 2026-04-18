# helios::gameplay::matchstate::rules

Transition rules for the match state machine.

## Rules

| Class | Description |
|-------|-------------|
| `DefaultMatchStateTransitionRules` | Provides the default set of match state transition rules |
| `DefaultMatchStateTransitionGuards` | Guard callbacks for conditional transitions |

## DefaultMatchStateTransitionRules

Defines standard transitions:

| From | Transition | To | Guard |
|------|------------|-----|-------|
| Undefined | WarmupRequest | Warmup | — |
| Warmup | StartRequest | Start | — |
| Start | CountdownRequest | Countdown | `isPlayerActive` |
| Countdown | PlayerSpawnRequest | Playing | — |
| Playing | PlayerDied | PlayerDefeated | — |
| PlayerDefeated | CountdownRequest | Countdown | `hasLifeLeft` |
| PlayerDefeated | GameOverRequest | GameOver | `hasNoLifeLeft` |
| GameOver | WarmupRequest | Warmup | — |
| GameOver | StartRequest | Start | — |

Additional cancel/reset transitions allow returning to earlier states from most active states (e.g., `Playing → Start`, `Countdown → Warmup`).

Custom rules can be provided to the `MatchStateManager` constructor.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::matchstate::rules
@brief Transition rules for the match state machine.
@details Contains rule definitions for valid state transitions.
</p></details>
