# helios::engine::modules::rendering::viewport::types

Types and policies for viewport management.

## Types

| Type | Purpose |
|------|---------|
| `StateToViewportPolicy` | Maps game/match states to viewport IDs |

## StateToViewportPolicy

Defines which viewports should be active for specific game and match state combinations. States are treated as bitmasks, allowing a single viewport to be associated with multiple states.

**Usage:**
```cpp
StateToViewportPolicy policy;
policy.add(GameState::Running, gameViewportId)
      .add(GameState::Paused, menuViewportId)
      .add(MatchState::Playing, hudViewportId);

// Query active viewports
auto viewports = policy.viewportIds(GameState::Running, MatchState::Playing);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::rendering::viewport::types
@brief Types and policies for viewport management.
@details Provides configuration types for state-based viewport management, including StateToViewportPolicy which maps game/match states to active viewports.
</p></details>

