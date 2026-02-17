# helios::engine::modules::rendering::viewport::components

Components for viewport state management.

## Components

| Component | Purpose |
|-----------|---------|
| `ActiveViewportIdsStateComponent` | Stores currently active viewport IDs |

## ActiveViewportIdsStateComponent

Maintains the list of viewport IDs that should be rendered for the current game/match state. Updated each frame by `StateToViewportPolicyUpdateSystem`.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::rendering::viewport::components
@brief Components for viewport state management.
@details Provides components that store viewport-related state, such as the currently active viewport IDs based on game/match state.
</p></details>

