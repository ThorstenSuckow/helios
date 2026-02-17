# helios::engine::modules::scene::systems

Scene graph synchronization and rendering systems.

## Systems

| System | Purpose |
|--------|---------|
| `SceneSyncSystem` | Propagates transforms from gameplay components to scene nodes |
| `SceneRenderingSystem` | Renders scenes through their associated viewports |

## SceneSyncSystem

Bridges the gameplay simulation layer with the rendering scene graph. Propagates transformation data from gameplay components to SceneNodes and reads back computed world transforms.

## SceneRenderingSystem

Iterates over all active viewport IDs from the session, resolves the associated scene and viewport via SceneToViewportMap, creates a scene snapshot, and submits it to the rendering device.

**Registration:**
```cpp
systemRegistry.add<SceneRenderingSystem>(
    Phase::Post,
    renderingDevice,
    sceneToViewportMap
);
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.modules.scene.types` | SceneToViewportMap |
| `helios.rendering` | RenderingDevice, RenderPass |
| `helios.scene` | Scene graph |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::scene::systems
@brief Scene graph synchronization and rendering systems.
@details Contains systems that bridge gameplay simulation and the rendering scene graph, including transform propagation and viewport-based scene rendering.
</p></details>
