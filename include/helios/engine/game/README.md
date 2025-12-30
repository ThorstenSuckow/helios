# helios::engine::game

Game logic infrastructure using the Command pattern for decoupled, testable, and deterministic game systems.

This module provides the foundational architecture for game logic in helios. It follows a composition-based component model where `GameObject` instances own `Component` instances that define behavior. Game actions are encapsulated as `Command` objects, enabling features like replay systems, undo/redo, and network synchronization.

The `GameWorld` serves as the central registry for all entities and systems. It manages:
- **GameObjects**: Individual game entities identified by unique `Guid`.
- **Systems**: Global logic processors for cross-cutting concerns (physics, collision, pooling).
- **GameObjectPool**: Memory-efficient object pooling for frequently spawned/despawned entities.

Systems are updated each frame after all GameObjects, providing a clear execution order.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game
@brief Game logic infrastructure using Command pattern for decoupled, deterministic game systems.
@details This namespace provides game logic infrastructure using the Command pattern for decoupled, testable, and deterministic game systems. It includes entity management (GameWorld, GameObject), system infrastructure (System), input processing (InputSnapshot), component system (Component, Updatable), and command execution (Command, CommandBuffer) facilities that enable advanced features like replay systems and undo/redo.
</p></details>
