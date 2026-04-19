# helios::rendering::concepts

Concepts for render-layer type constraints.

| Concept | Description |
|---------|-------------|
| `IsRenderBackendLike<T>` | Constrains backend API shape (`beginRenderPass`, `endRenderPass`, `init`, `initialized`) |
| `IsRenderResourceHandle<T>` | Constrains resource handles (`ShaderHandle`, `MaterialHandle`, `MeshHandle`) |
| `IsRenderTargetHandle<T>` | Constrains target handles (`ViewportHandle`, `FramebufferHandle`) |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::concepts
@brief Concepts for render-layer type constraints.
@details Provides compile-time constraints for rendering backends and handle categories.
</p></details>

