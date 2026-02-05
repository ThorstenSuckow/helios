# helios::engine::mechanics::combat::types

Combat-related data types and structures.

## Types

| Type | Description |
|------|-------------|
| `AttackContext` | Data structure containing attack information |

## AttackContext

Stores information about an attack event:

| Field | Type | Description |
|-------|------|-------------|
| `source` | `Guid` | GUID of the attacking entity |
| `contact` | `vec3f` | World-space contact point |
| `damageApplied` | `float` | Net damage after modifiers |

Used by:
- `LastAttackerComponent` - stores the most recent attack
- `DeathEvent` - provides attack context for kill attribution
- `CombatScoringSystem` - determines who gets the kill reward

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat::types
@brief Combat-related data types and structures.
@details Contains data structures for representing attack information used in damage processing and kill attribution.
</p></details>
