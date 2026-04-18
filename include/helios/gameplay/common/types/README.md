# helios::gameplay::common::types

Shared context structs for entity-to-entity interactions.

## Types

| Type | Description |
|------|-------------|
| `InteractionContext` | Base context carrying target, instigator, causer, and contact point |
| `DamageContext` | Extends InteractionContext with a damage amount |

## InteractionContext

Generic interaction between two entities:
- `target` – Entity affected by the interaction
- `instigator` – Entity that initiated the interaction
- `causer` – Entity that directly caused the interaction (e.g. projectile)
- `contact` – World-space contact point

## DamageContext

Builds on InteractionContext by adding:
- `interactionContext` – The underlying interaction
- `damage` – Amount of damage to apply

Used by `HealthManager` and `ApplyDamageCommand` to transport damage
information from the collision/combat layer to the health system.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::common::types
@brief Shared context structs for entity-to-entity interactions.
@details Contains InteractionContext and DamageContext used across combat, health, and scoring subsystems.
</p></details>

