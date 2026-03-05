# helios::engine::mechanics::lifecycle::commands

World-level lifecycle commands for the command buffer.

This namespace contains command classes used to request world-level lifecycle operations through the command system.

## Commands

| Command | Description |
|---------|-------------|
| `WorldLifecycleCommand` | Carries a `WorldLifecycleAction` to request a world-level operation (e.g. Reset) |

`WorldLifecycleAction` is defined in the sibling `types` submodule (`helios.engine.mechanics.lifecycle.types`).

## Usage

Commands are typically issued by state listeners when the game world needs to be reset:

```cpp
updateContext.queueCommand<WorldLifecycleCommand>(WorldLifecycleAction::Reset);
```

The command is routed through the `EngineCommandBuffer` to the `WorldLifecycleManager`, which executes the requested action during its flush cycle.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::lifecycle::commands
@brief World-level lifecycle commands for the command buffer.
@details Contains command classes for requesting world-level lifecycle operations through the engine's command system.
</p></details>


