# helios::platform::glfw

GLFW-backed platform integration.

## Overview

This module provides the concrete GLFW implementation used by the platform
layer, including a manager plus GLFW-specific bridge components/types/systems.

## Key Types

| Type | Purpose |
|------|---------|
| `GLFWPlatformManager<THandle>` | Concrete manager handling GLFW init/window/event integration |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `components/` | GLFW-native handles and user pointer bridge components |
| `systems/` | GLFW-specific window-close integration systems |
| `types/` | GLFW wrapper value types |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::glfw
@brief GLFW-backed platform integration.
</p></details>

