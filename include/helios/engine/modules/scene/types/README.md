# helios::engine::modules::scene::types

Types and data structures for scene management.

## Types

| Type | Purpose |
|------|---------|
| `SceneToViewportMap` | Maps scenes to their associated viewports |

## SceneToViewportMap

Maintains the relationship between Scene objects and Viewport objects for rendering. A scene can be rendered through multiple viewports (e.g., split-screen), and the map provides O(1) lookups after cache updates.

**Usage:**
```cpp
SceneToViewportMap map;
map.add(&gameScene, &mainViewport);
map.add(&uiScene, &hudViewport);

// Query scene for viewport
auto* scene = map.scene(viewportId);

// Query all viewports for a scene
auto viewports = map.viewports(sceneId);
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.modules.scene.systems` | Systems using the scene-to-viewport mapping |
| `helios.scene` | Scene graph types |
| `helios.rendering` | Viewport definitions |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::scene::types
@brief Types and data structures for scene management.
@details Provides data structures for mapping scenes to viewports, enabling state-based rendering of different scene configurations.
</p></details>

