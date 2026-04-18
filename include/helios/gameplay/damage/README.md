# helios::gameplay::damage

Damage dealing system for game entities.

This module provides components and systems for applying damage to entities based on collision events.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `commands/` | Commands for requesting damage application |
| `components/` | Damage-related ECS components |
| `systems/` | Systems for damage processing |

## Architecture

1. **DamageDealerComponent** defines damage values per collision layer
2. **DamageOnCollisionSystem** listens for `SolidCollisionEvent`
3. When collision occurs, an `ApplyDamageCommand` is issued with a `DamageContext`
4. **HealthManager** receives the command and applies damage to the target
5. **LastDamageComponent** is updated with the damage context for attribution

## Integration

The damage system integrates with:
- **Collision system** - receives `SolidCollisionEvent` for damage triggers
- **Health system** - applies damage via `HealthComponent.takeDamage()`
- **Combat system** - updates `LastDamageComponent` for kill attribution
- **Spawn system** - uses `EmittedByComponent` to track projectile sources

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::damage
@brief Damage dealing system for game entities.
@details Provides infrastructure for defining layer-based damage values and applying damage on collision.
</p></details>
