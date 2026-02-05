# helios::engine::mechanics::health

Health management system for game entities.

This module provides components, events, and systems for tracking entity health, processing damage, and handling death.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Health-related ECS components |
| `events/` | Health-related events (death) |
| `systems/` | Systems for health processing |

## Architecture

1. **HealthComponent** tracks current and maximum health
2. **Combat systems** call `takeDamage()` when damage is dealt
3. **HealthUpdateSystem** detects `health <= 0` and:
   - Issues `DespawnCommand` to remove the entity
   - Publishes `DeathEvent` for other systems (e.g., scoring)

## Integration

The health system integrates with:
- **Combat system** - receives damage via `takeDamage()`
- **Scoring system** - `DeathEvent` triggers score awards
- **Spawn system** - dead entities are despawned via command

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::health
@brief Health management system for game entities.
@details Provides infrastructure for tracking entity health, processing damage and healing, and handling entity death through events.
</p></details>
