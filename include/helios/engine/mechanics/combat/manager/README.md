# helios::engine::mechanics::combat::manager

Managers for combat-related deferred operations.

## Overview

This module provides manager implementations for handling combat operations that must be processed at specific points in the game loop. Managers queue requests and process them during the manager flush phase.

## Key Classes

| Class | Purpose |
|-------|---------|
| `ProjectilePoolManager` | Manages projectile spawning from pools based on shoot requests |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat::manager
@brief Managers for combat-related deferred operations.
@details Provides managers that handle projectile pooling and spawning during the game loop's manager flush phase.
</p></details>

