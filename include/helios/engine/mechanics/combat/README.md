# helios::engine::mechanics::combat

Combat-related gameplay systems, components, and commands.

## Overview

This module provides all building blocks for implementing combat mechanics such as aiming, shooting, and projectile management.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Data containers for combat state (aiming, cooldowns) |
| `commands/` | Commands for combat actions (ShootCommand, Aim2DCommand) |
| `dispatcher/` | Command dispatchers routing to managers |
| `manager/` | Managers for deferred projectile operations |
| `requests/` | Request types for projectile spawning |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat
@brief Combat-related gameplay systems, components, and commands.
@details This namespace contains the combat layer of the gameplay system. It includes components for storing weapon and aiming state, commands for translating input into combat actions, dispatchers for routing commands, managers for deferred projectile operations, and request types for spawn communication.
</p></details>

