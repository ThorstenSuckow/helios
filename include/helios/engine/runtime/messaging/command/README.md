# helios::engine::runtime::messaging::command

Command pattern implementation for deferred action execution.

This module provides the Command pattern implementation for the helios engine. Commands encapsulate game actions that can be buffered and executed at a later point in the game loop, enabling decoupled input handling and action processing.

## Key Classes

| Class | Purpose |
|-------|---------|
| `TargetedCommand` | Abstract base for commands targeting a specific GameObject |
| `WorldCommand` | Abstract base for commands operating on the entire GameWorld |
| `CommandBuffer` | Queue for buffering and batch-executing commands |
| `TargetedCommandDispatcher` | Abstract dispatcher interface for TargetedCommand handling |
| `WorldCommandDispatcher` | Abstract dispatcher interface for WorldCommand handling |

## Execution Order

During `CommandBuffer::flush()`:
1. **WorldCommands** are processed first (global actions like spawning)
2. **TargetedCommands** are processed second (per-entity actions like movement)

## Usage

```cpp
// 1. Create command and enqueue via UpdateContext
auto moveCmd = std::make_unique<Move2DCommand>(
    playerGuid,
    helios::math::vec2f{1.0f, 0.0f},  // direction
    1.0f  // throttle
);
updateContext.commandBuffer().enqueue(std::move(moveCmd));

// 2. Register dispatcher for command type
auto moveDispatcher = std::make_unique<TypedTargetedCommandDispatcher<Move2DCommand>>(
    [&](const Move2DCommand& cmd, GameObject target) {
        auto* move = target->get<Move2DComponent>();
        move->setDirection(cmd.direction());
        move->setThrottle(cmd.throttle());
    }
);
commandBuffer.registerDispatcher(std::move(moveDispatcher));

// 3. Commands are automatically flushed at phase boundaries
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::messaging::command
@brief Command pattern implementation for deferred action execution.
@details Provides TargetedCommand and WorldCommand base classes for encapsulating game actions. Commands are buffered in CommandBuffer and executed during the game loop update phase. Dispatchers enable type-safe routing to specialized handlers via the Visitor pattern.
</p></details>


