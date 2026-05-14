# helios::rendering::common::concepts

Concepts for render-layer type constraints.

## Overview

These concepts provide small compile-time checks for backend interfaces and
rendering handle categories used across the rendering layer.

## Concepts

| Concept | Description |
|---------|-------------|
| `IsRenderBackendLike<T, THandle>` | Constrains backend API shape (`beginRenderPass`, `doRender`, `endRenderPass`) |
| `IsRenderResourceHandle<T>` | Constrains resource handles (`ShaderHandle`, `MaterialHandle`, `MeshHandle`) |
| `IsRenderTargetHandle<T>` | Constrains target handles (`ViewportHandle`, `FramebufferHandle`) |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::common::concepts
@brief Concepts for render-layer type constraints.
@details Provides compile-time constraints for rendering backends and handle categories.
</p></details>

