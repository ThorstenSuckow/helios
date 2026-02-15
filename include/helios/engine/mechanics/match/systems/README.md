# helios::engine::mechanics::match::systems

Systems for match state management.

## Systems

| System | Description |
|--------|-------------|
| `MatchFlowSystem` | Automatically advances the match through state phases |
| `MatchRuleSystem` | Evaluates match rules and conditions |

## MatchFlowSystem

Drives automatic state progression through:
- Undefined → Warmup
- Warmup → PlayerSpawn
- PlayerSpawn → Playing

## MatchRuleSystem

Placeholder system for evaluating match rules such as win/lose conditions, time limits, and score thresholds.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::match::systems
@brief Systems for match state management.
@details Contains ECS systems that handle match flow and rule logic.
</p></details>

