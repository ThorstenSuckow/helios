# helios::engine::runtime::messaging

Communication infrastructure for commands and events within the game loop.

This module provides the messaging backbone for the helios engine. It includes the Command pattern implementation for buffering and executing game actions, as well as event buses for decoupled inter-system communication.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `command/` | Command buffer, dispatchers, and command types |
| `event/` | Game loop event bus for phase/pass/frame events |

## Key Classes

### Command

| Class | Purpose |
|-------|---------|
| `CommandBuffer` | Collects commands and flushes to dispatchers |
| `WorldCommand` | Command operating on GameWorld state |
| `TargetedCommand` | Command targeting a specific GameObject |
| `WorldCommandDispatcher` | Routes world commands to handlers |
| `TargetedCommandDispatcher` | Routes targeted commands to handlers |

### Event

| Class | Purpose |
|-------|---------|
| `GameLoopEventBus` | Double-buffered event bus with pass/phase/frame scopes |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::messaging
@brief Communication infrastructure for commands and events.
@details This module provides the messaging backbone for the game loop, including command buffering for deferred action execution and event buses for inter-system communication.
</p></details>


