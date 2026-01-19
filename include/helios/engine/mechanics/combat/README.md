# helios::engine::mechanics::combat

Combat-related gameplay systems, components, and commands.

## Overview

This module provides all building blocks for implementing combat mechanics such as aiming, shooting, and projectile spawning.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `commands/` | Commands for combat actions (ShootCommand, Aim2DCommand) |
| `components/` | Data containers for combat state (aiming direction, cooldowns, projectile config) |
| `systems/` | ECS systems for processing combat logic (ProjectileSpawnSystem) |

## Architecture

Combat follows the command pattern integrated with the spawn system:

1. Input systems generate `ShootCommand` and `Aim2DCommand`
2. Commands update `ShootComponent` and `Aim2DComponent` state
3. `ProjectileSpawnSystem` reads component state and generates `SpawnCommand`
4. Spawn system handles actual projectile creation via pool and profile

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat
@brief Combat-related gameplay systems, components, and commands.
@details This namespace contains the combat layer of the gameplay system. It includes components for storing weapon and aiming state, commands for translating input into combat actions, and systems for spawning projectiles based on combat state.
</p></details>

