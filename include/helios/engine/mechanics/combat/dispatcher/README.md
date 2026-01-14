# helios::engine::mechanics::combat::dispatcher

Command dispatchers for combat-related commands.

## Overview

This module provides dispatcher implementations for routing combat commands to their appropriate handlers. Dispatchers connect the command buffer to managers that process shooting and projectile-related operations.

## Key Classes

| Class | Purpose |
|-------|---------|
| `ShootCommandDispatcher` | Routes ShootCommand instances to the ProjectilePoolManager |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat::dispatcher
@brief Command dispatchers for combat-related commands.
@details Provides dispatchers that route combat commands from the CommandBuffer to their respective managers.
</p></details>

