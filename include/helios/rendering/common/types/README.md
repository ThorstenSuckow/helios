# helios::rendering::common::types

Shared rendering value types used across common render flow APIs.

## Overview

`helios::rendering::common::types` defines small value types shared between
render extraction, render pass setup, and backend execution code.

## Types

| Type | Description |
|------|-------------|
| `ClearFlags` | Bitmask enum for color, depth, and stencil clear operations |
| `RenderPassContext` | Small context object that groups framebuffer and viewport handles for a render pass |

## Notes

- `ClearFlags` starts with `ClearFlags::None` and is intended to be combined as a bitmask.
- `RenderPassContext` currently stores `FramebufferHandle` and `ViewportHandle`.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::common::types
@brief Shared rendering value types used across common render flow APIs.
@details Defines lightweight common types such as clear flags and render pass
context data shared between rendering modules.
</p></details>

