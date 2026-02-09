# helios::engine::mechanics

High-level gameplay mechanics and systems for game logic.

## Overview

This module provides the core building blocks for implementing game mechanics. It contains components that store game state and systems that implement game rules according to the composition-over-inheritance principle.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `bounds/` | Level boundary behavior and response systems |
| `combat/` | Weapon, shooting, attack tracking |
| `damage/` | Damage dealing and damage application |
| `health/` | Health tracking and death detection |
| `lifecycle/` | Entity activation state (Active/Inactive tags) |
| `scoring/` | Score pools, score values, and score tracking |
| `spawn/` | Entity spawning and lifecycle management |
| `input/` | Input processing systems (twin-stick controllers) |

## Key Components

| Component | Purpose |
|-----------|---------|
| `Active` / `Inactive` | Tag components for entity activation state |
| `HealthComponent` | Tracks current/max health, alive status |
| `DamageDealerComponent` | Stores layer-based damage values |
| `LastAttackerComponent` | Tracks last entity that dealt damage |
| `SpawnedByProfileComponent` | Links entity to spawn profile for pooling |
| `EmittedByComponent` | Tracks source entity (e.g., player → projectile) |
| `ScoreValueComponent` | Score awarded when entity is defeated |
| `ScorePoolComponent` | Associates entity with a score pool |

## Key Systems

| System | Purpose |
|--------|---------|
| `TwinStickInputSystem` | Translates gamepad input to movement/aim commands |
| `LevelBoundsSystem` | Applies bounce/wrap/despawn at level edges |
| `HealthDepletedSystem` | Detects death and triggers death events |
| `DamageApplicationSystem` | Applies damage from collision events |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics
@brief High-level gameplay systems and components for game logic.
@details This namespace contains the gameplay layer of the engine, providing components that store game state and systems that implement game rules. Components are pure data containers; systems read and modify them each frame. The namespace is subdivided into `bounds` for level boundary behavior, `combat` for weapon and attack tracking, `damage` for damage dealing, `health` for health tracking, `lifecycle` for entity activation, `scoring` for score management, `spawn` for entity lifecycle management, and `input` for input processing systems.
</p></details>
