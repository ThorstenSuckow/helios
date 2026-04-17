# helios::platform

Platform abstraction layer for window/runtime integration.

## Overview

`helios::platform` provides platform-facing modules used to bootstrap and drive
application runtime behavior (window creation, event polling, context lifecycle,
and backend-specific integration).

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `concepts/` | Shared platform handle concepts |
| `runtime/` | Platform runtime entities, commands, systems, and state components |
| `window/` | Window-domain handles, commands, components, and systems |
| `glfw/` | GLFW-backed platform manager and glue components/systems |
| `opengl/` | OpenGL-specific platform managers/components |

## See Also

- [Runtime](runtime/README.md)
- [Window](window/README.md)
- [GLFW](glfw/README.md)
- [OpenGL](opengl/README.md)

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform
@brief Platform abstraction layer for runtime and window integration.
</p></details>

