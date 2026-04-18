# helios::runtime::messaging

Communication infrastructure for commands and events within the game loop.

This module provides the messaging backbone for the helios engine. It includes the compile-time typed Command system for buffering and routing game actions to handlers, as well as event buses for decoupled inter-system communication.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `command/` | Compile-time typed command buffer, handler routing, and EngineCommandBuffer |
| `event/` | Game loop event bus for phase/pass/frame events |

## Key Classes

### Command

| Class | Purpose |
|-------|---------|
| `CommandBuffer` | Abstract base for command buffers |
| `TypedCommandBuffer<...Cmds>` | Compile-time typed buffer with per-type queues |
| `EngineCommandBuffer` | Concrete facade with all engine command types |
| `CommandHandlerRegistry` | Registry storing typed submit functions |

### Event

| Class | Purpose |
|-------|---------|
| `GameLoopEventBus` | Double-buffered event bus with pass/phase/frame scopes |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::runtime::messaging
@brief Communication infrastructure for commands and events.
@details Provides compile-time typed command buffering with handler routing, and double-buffered event buses for decoupled inter-system communication within the game loop.
</p></details>


