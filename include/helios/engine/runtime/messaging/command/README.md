# helios::engine::runtime::messaging::command

Command pattern implementation for deferred action execution.

This module provides the Command pattern implementation for the helios engine. Commands encapsulate game actions that can be buffered and executed at a later point in the game loop, enabling decoupled input handling and action processing.

**Components:**

- **TargetedCommand** - Abstract base for commands targeting a specific GameObject.
- **WorldCommand** - Abstract base for commands operating on the entire GameWorld.
- **CommandBuffer** - Queue for buffering and batch-executing commands.
- **TargetedCommandDispatcher** - Abstract dispatcher interface for TargetedCommand handling.
- **WorldCommandDispatcher** - Abstract dispatcher interface for WorldCommand handling.
- **TypedTargetedCommandDispatcher** - Type-safe dispatcher template for specific TargetedCommand types.
- **TypedWorldCommandDispatcher** - Type-safe dispatcher template for specific WorldCommand types.

**Execution order:**

During `CommandBuffer::flush()`:
1. **WorldCommands** are processed first (global game actions like spawning).
2. **TargetedCommands** are processed second (per-entity actions like movement).

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::messaging::command
@brief Command pattern implementation for deferred action execution.
@details Provides TargetedCommand and WorldCommand base classes for encapsulating game actions. Commands are buffered in CommandBuffer and executed during the game loop update phase. Dispatchers enable type-safe routing to specialized handlers via the Visitor pattern.
</p></details>


