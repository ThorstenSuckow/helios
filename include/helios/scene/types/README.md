# helios::scene::types

Type layer for scene-domain handles and render/camera contexts.

## Overview

`helios::scene::types` contains strong-id aliases and immutable context payloads
used across scene extraction, culling, and rendering integration.

## Types

| Type | Purpose |
|------|---------|
| `SceneId` | Strong id for scene entities |
| `SceneHandle` | ECS handle alias for scene entities |
| `PerspectiveCameraContext` | Perspective projection parameters used by culling/rendering |
| `SceneMemberRenderContext<THandle>` | Render payload for one scene member in one viewport |

## Related Modules

- `helios.scene.Scene`
- `helios.scene.SceneNode`
- `helios.scene.SceneToViewportMap`

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::scene::types
@brief Type layer for scene-domain handles and render/camera contexts.
</p></details>

