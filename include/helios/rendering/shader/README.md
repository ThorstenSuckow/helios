# helios::rendering::shader

Shader-domain abstractions and resource types.

This module defines the shader API (`Shader`), uniform semantics/value transport,
resource entity aliases, and submodules for commands, components, concepts,
systems, and types.

## Contents

| Item | Description |
|------|-------------|
| `Shader` | Backend-agnostic shader interface (`use`, uniform application) |
| `UniformSemantics` | Semantic keys used to address shader uniforms |
| `UniformValueMap` | Typed container for uniform payload values |
| `ShaderEntity` | ECS facade alias for shader resource entities |
| `ShaderEntityManager` | ECS manager alias for shader resources |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `commands/` | Shader-related command payload types |
| `components/` | ECS components for shader resources |
| `concepts/` | Type constraints for shader handle usage |
| `systems/` | Runtime systems operating on shader entities |
| `types/` | Strong ids and handle aliases |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::shader
@brief Shader-domain abstractions and resource types.
@details Provides the core shader interface, uniform semantics/value mapping, and ECS resource aliases. Functional concerns are split into commands, components, concepts, systems, and types submodules.
</p></details>

