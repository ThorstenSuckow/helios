# helios::engine::game::gameplay

High-level gameplay systems and components for game logic.

This module provides the core building blocks for implementing game mechanics. It is organized into two sub-namespaces:

- **components**: Data containers for gameplay state such as aiming direction, shooting cooldowns, and projectile configuration.
- **systems**: Update logic that operates on gameplay components, including spawning and projectile pool management.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::gameplay
@brief High-level gameplay systems and components for game logic.
@details This namespace contains the gameplay layer of the engine, providing components that store game state and systems that implement game rules. Components are pure data containers; systems read and modify them each frame.
</p></details>