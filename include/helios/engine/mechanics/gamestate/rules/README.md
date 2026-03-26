# helios::engine::mechanics::gamestate::rules

Transition rules for the game state machine.

## Rules

| Class | Description |
|-------|-------------|
| `DefaultGameStateTransitionRules` | Provides the default set of transition rules |

## DefaultGameStateTransitionRules

Defines standard transitions:

| From | Transition | To |
|------|------------|-----|
| Undefined | BootRequest | Booted |
| Booted | TitleRequest | Title |
| Title | ReadyMatchRequest | MatchReady |
| MatchReady | StartMatchRequest | Running |
| Running | TogglePause | Paused |
| Paused | TogglePause | Running |
| Paused | TitleRequest | Title |
| Paused | ReadyMatchRequest | MatchReady |
| Running | TitleRequest | Title |
| Running | ReadyMatchRequest | MatchReady |

Custom rules can be provided to the `GameStateManager` constructor.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate::rules
@brief Transition rules for the game state machine.
@details Contains rule definitions for valid state transitions.
</p></details>
