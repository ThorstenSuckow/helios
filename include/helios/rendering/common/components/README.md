# helios::rendering::common::components

Shared ECS components used across rendering workflows.

## Overview

This module contains small reusable components that describe generic rendering
state independent of a specific backend or resource subsystem.

## Components

| Component | Description |
|-----------|-------------|
| `ClearComponent<THandle>` | Stores clear flags for a render-related entity |
| `MaterialOverrideComponent<THandle>` | Stores an explicit material override handle |
| `RenderableComponent<THandle>` | Binds an entity to a renderable resource |
| `RenderPrototypeComponent<THandle>` | Groups mesh, shader, and material handles |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::common::components
@brief Shared ECS components used across rendering workflows.
</p></details>


