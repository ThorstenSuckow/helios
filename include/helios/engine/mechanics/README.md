# helios::engine::mechanics

High-level gameplay mechanics and systems for game logic.

## Overview

This module provides the core building blocks for implementing game mechanics. It contains components that store game state and systems that implement game rules according to the composition-over-inheritance principle.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `bounds/` | Level boundary behavior and response systems |
| `combat/` | Weapon, shooting, and projectile mechanics |
| `spawn/` | Entity spawning and lifecycle management |
| `input/` | Input processing systems (twin-stick controllers) |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics
@brief High-level gameplay systems and components for game logic.
@details This namespace contains the gameplay layer of the engine, providing components that store game state and systems that implement game rules. Components are pure data containers; systems read and modify them each frame. The namespace is subdivided into `bounds` for level boundary behavior, `combat` for weapon and projectile mechanics, `spawn` for entity lifecycle management, and `input` for input processing systems.
</p></details>
