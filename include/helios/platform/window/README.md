# helios::platform::window

Window-domain platform abstractions.

## Overview

This module encapsulates window handles/ids, window commands, ECS components,
and systems for creating windows and swapping buffers.

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `commands/` | Window commands (create, resize, close, swap buffers) |
| `components/` | ECS components representing window state and creation requests |
| `concepts/` | Handle constraints for window domain types |
| `systems/` | Systems processing window commands and frame swap behavior |
| `types/` | Window strong ids, handles, sizes, and config types |

## Key Types

| Type | Purpose |
|------|---------|
| `WindowEntityManager` | ECS manager alias for window entities |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::window
@brief Window-domain platform abstractions.
</p></details>

