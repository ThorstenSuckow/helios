# helios::gameplay::damage::components

Damage-related ECS components.

## Components

| Component                | Description                                        |
|--------------------------|----------------------------------------------------|
| `DamageDealerComponent`  | Component storing damage values per collision layer |
| `LastDamageComponent` | Component storing last damage applied              |

## DamageDealerComponent

Stores damage values indexed by collision layer for O(1) lookup:

- `setDamage(float, uint32_t)` - Sets damage for a specific layer
- `damage(uint32_t)` - Returns damage for a specific layer

Layer IDs must be powers of 2 (bitmask values).

## LastDamageComponent
Stores the last damage applied to an entity.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::damage::components
@brief Damage-related ECS components.
@details Contains components for defining damage values per collision layer.
</p></details>
