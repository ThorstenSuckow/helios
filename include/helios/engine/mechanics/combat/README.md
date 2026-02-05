# helios::engine::mechanics::combat

Combat-related gameplay systems, components, and commands.

## Overview

This module provides all building blocks for implementing combat mechanics such as aiming, shooting, projectile spawning, and attack attribution.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `commands/` | Commands for combat actions (ShootCommand, Aim2DCommand) |
| `components/` | Data containers for combat state (aiming, cooldowns, attack tracking) |
| `systems/` | ECS systems for processing combat logic (ProjectileSpawnSystem) |
| `types/` | Combat data types (AttackContext) |

## Key Classes

| Class | Purpose |
|-------|---------|
| `Aim2DComponent` | Stores aiming direction and fire frequency |
| `ShootComponent` | Weapon cooldown, spawn profile reference, emitter state |
| `LastAttackerComponent` | Tracks last entity that attacked this entity |
| `AttackContext` | Data struct with attacker info for kill attribution |
| `Aim2DCommand` | Updates aim direction from input |
| `ShootCommand` | Triggers projectile spawn when cooldown allows |
| `ProjectileSpawnSystem` | Reads shoot state and enqueues spawn commands |

## Architecture

Combat follows the command pattern integrated with the spawn system:

1. Input systems generate `ShootCommand` and `Aim2DCommand`
2. Commands update `ShootComponent` and `Aim2DComponent` state
3. `ProjectileSpawnSystem` reads component state and generates `SpawnCommand`
4. Spawn system handles actual projectile creation via pool and profile

### Attack Attribution Flow

1. `DamageOnCollisionSystem` applies damage and updates `LastAttackerComponent`
2. `HealthUpdateSystem` detects death and publishes `DeathEvent` with `AttackContext`
3. `CombatScoringSystem` reads `DeathEvent` and awards score to attacker

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat
@brief Combat-related gameplay systems, components, and commands.
@details This namespace contains the combat layer of the gameplay system. It includes components for storing weapon and aiming state, commands for translating input into combat actions, systems for spawning projectiles, and types for attack attribution.
</p></details>

