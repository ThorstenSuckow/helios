# helios::gameplay::health::systems

Systems for health processing.

## Systems

| System | Description |
|--------|-------------|
| `HealthUpdateClearSystem` | Clears the dirty flag on all active HealthComponents |


## HealthUpdateClearSystem

Iterates all active `HealthComponent`s and calls `clearDirty()`, ensuring
stale change notifications are discarded after observers have processed them.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::health::systems
@brief Systems for health processing.
@details Contains systems that clear dirty state.
</p></details>
