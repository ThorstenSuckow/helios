# helios::rendering::mesh

This namespace provides mesh rendering abstractions for 3D geometry. It defines the core types for submitting mesh draw commands to the render queue.

| Class | Description |
|-------|-------------|
| `MeshRenderable` | High-level component combining a shared `RenderPrototype` with per-instance material overrides |
| `MeshRenderCommand` | Low-level, move-only command object consumed by the rendering device |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::mesh
@brief Mesh rendering abstractions and command types.
@details This namespace provides the core types for mesh rendering. `MeshRenderable` is a high-level component that can be attached to game objects, combining a shared `RenderPrototype` with optional per-instance `MaterialShaderPropertiesOverride`. `MeshRenderCommand` is the low-level command object that encapsulates all data needed for a single mesh draw call.
</p></details>
