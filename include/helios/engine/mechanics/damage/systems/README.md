# helios::engine::mechanics::damage::systems

Systems for damage processing.

## Systems

| System | Phase | Description |
|--------|-------|-------------|
| `DamageOnCollisionSystem` | Update | Applies damage when solid collisions occur |

## DamageOnCollisionSystem

Processes `SolidCollisionEvent` and applies damage:

1. Reads collision events from the pass buffer
2. Checks if source entity has `DamageDealerComponent`
3. Checks if target entity has `HealthComponent`
4. Looks up damage value by target's collision layer
5. Calls `takeDamage()` on target's health component
6. Updates `LastAttackerComponent` with attack context

The attack source is resolved through `EmittedByComponent` if present (e.g., projectile → player).

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::damage::systems
@brief Systems for damage processing.
@details Contains systems that apply damage based on collision events.
</p></details>
