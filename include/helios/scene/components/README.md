# helios::scene::components

Scene graph and camera integration components.

This module contains components that bridge game objects, scenes, cameras,
viewports, and derived render data.

## Components

| Component | Description |
|-----------|-------------|
| `CameraBindingComponent` | Binds an entity to a camera game object handle. |
| `ProjectionMatrixComponent` | Stores a computed projection matrix. |
| `ViewMatrixComponent` | Stores a computed view matrix. |
| `LookAtComponent` | Stores look-at target and up vector data. |
| `PerspectiveCameraComponent` | Stores perspective projection parameters. |
| `SceneBindingComponent` | Binds an entity to a scene handle. |
| `SceneMemberComponent` | Marks an entity as a member of a scene. |
| `SceneNodeComponent` | Links a GameObject to a SceneNode for rendering. |
| `ViewportComponent` | Groups viewport, scene, and camera bindings. |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::scene::components
@brief Scene graph and camera integration components.
@details This module contains components that bridge game objects, scenes,
camera bindings, viewport bindings, and derived render data used by the scene
layer.
</p></details>