# helios::engine::mechanics::combat::systems

Gameplay systems for combat mechanics processing.

## Overview

This module contains systems that process combat-related components each frame. Systems read component state and generate commands or modify world state accordingly.

## Key Classes

| Class | Purpose |
|-------|---------|
| `ProjectileSpawnSystem` | Spawns projectiles for entities with active ShootComponents |

## System Flow

The `ProjectileSpawnSystem` operates as follows:

1. Queries entities with `ShootComponent`, `Aim2DComponent`, and `TranslationStateComponent`
2. Checks if shooting is active (intensity > 0) and aim direction is valid
3. Calculates projectile velocity from source velocity and aim direction
4. Enqueues `SpawnCommand` with `EmitterContext` for deferred spawning

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat::systems
@brief Gameplay systems for combat mechanics processing.
@details Contains systems that iterate combat-related components and generate spawn commands for projectiles based on shooting state and aim direction.
</p></details>
