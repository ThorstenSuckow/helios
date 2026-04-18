# helios::gameplay::match::systems

Systems for match state management.

## Systems

| System | Description |
|--------|-------------|
| `MatchFlowSystem` | Automatically advances the match through state phases |
| `MatchRuleSystem` | Evaluates match rules and conditions |

## MatchFlowSystem

Drives automatic state progression through:
- Undefined → Warmup (WarmupRequest)
- Warmup → Start (StartRequest)
- Start → Countdown (CountdownRequest, guarded: player must be active)
- Countdown → Playing (PlayerSpawnRequest)
- Playing → PlayerDefeated (PlayerDied)
- PlayerDefeated → Countdown (CountdownRequest, guarded: has life left)
- PlayerDefeated → GameOver (GameOverRequest, guarded: no life left)

## MatchRuleSystem

Placeholder system for evaluating match rules such as win/lose conditions, time limits, and score thresholds.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::match::systems
@brief Systems for match state management.
@details Contains ECS systems that handle match flow and rule logic.
</p></details>
