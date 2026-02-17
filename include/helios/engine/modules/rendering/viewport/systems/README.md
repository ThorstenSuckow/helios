# helios::engine::modules::rendering::viewport::systems

Systems for viewport state updates.

## Systems

| System | Purpose |
|--------|---------|
| `StateToViewportPolicyUpdateSystem` | Updates active viewports based on state policy |

## StateToViewportPolicyUpdateSystem

Queries the current `GameState` and `MatchState` from the session, then uses the configured `StateToViewportPolicy` to determine which viewports should be active. The resulting viewport IDs are stored in the session for use by the rendering system.

**Registration:**
```cpp
systemRegistry.add<StateToViewportPolicyUpdateSystem>(
    Phase::Pre,
    std::move(policy)
);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::rendering::viewport::systems
@brief Systems for viewport state updates.
@details Provides systems that manage viewport visibility based on game state, updating the session's active viewport list each frame.
</p></details>

