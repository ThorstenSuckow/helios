# helios::engine::mechanics::match::components

Components for match rule tracking.

## Components

| Component | Description |
|-----------|-------------|
| `PlayerComponent` | Marker identifying the player entity |
| `LivesComponent` | Tracks remaining lives with revision-based change detection |
| `LivesBindingComponent` | Binds a UI entity to a target's LivesComponent |

## LivesComponent

Maintains a life counter and a monotonically increasing `LivesRevision`.
Each call to `decrease()` or `increase()` bumps the revision so observer
systems can detect changes without polling.

## LivesBindingComponent

Stores a target `EntityHandle` and a cached `LivesRevision`. The
`Lives2UiTextUpdateSystem` compares the cached revision against the
target's current revision to update the bound UI text on change.

## PlayerComponent

Empty marker component. Systems query for it to apply player-specific
logic (input handling, camera, UI).

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::match::components
@brief Components for match rule tracking.
@details Contains player identification, life tracking, and UI binding components.
</p></details>

