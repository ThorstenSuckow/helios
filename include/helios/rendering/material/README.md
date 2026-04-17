# helios::rendering::material

Material abstractions and shader property models.

This module provides immutable material definitions (`Material`,
`MaterialShaderProperties`) and optional instance-level override payloads.

| Item | Description |
|------|-------------|
| `Material` | Material object binding a shader with default material properties |
| `MaterialShaderProperties` | Immutable default material uniform values |
| `MaterialShaderPropertiesOverride` | Optional per-instance material override payload |
| `MaterialEntityManager` | ECS manager alias for material resources |
| `MaterialProperties` | Simple material property payload |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `types/` | Strong ids and handle aliases |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::material
@brief Material abstractions and shader property models.
@details Contains immutable material property models, override payloads, and ECS resource aliases for material resources.
</p></details>
