# helios::engine::game

Game logic infrastructure using the Command pattern for decoupled, testable, and deterministic game systems.

This module provides the foundational architecture for game logic in helios. It follows a composition-based component model where `GameObject` instances own `Component` instances that define behavior. Game actions are encapsulated as `Command` objects, enabling features like replay systems, undo/redo, and network synchronization.



---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game
@brief Game logic infrastructure using Command pattern for decoupled, deterministic game systems.
@details This namespace provides game logic infrastructure using the Command pattern for decoupled, testable, and deterministic game systems. It includes entity management (GameWorld, GameObject), input processing (InputSnapshot), component system (Component, Updatable), system infrastructure (System), and command execution (Command, CommandBuffer) facilities that enable advanced features like replay systems and undo/redo.
</p></details>
