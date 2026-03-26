# helios::engine::mechanics::timing::commands

Timer control commands for the command buffer.

This namespace contains command classes used to request timer state transitions through the command system.

## Commands

| Command | Description |
|---------|-------------|
| `TimerControlCommand` | Carries a `TimerControlContext` to request a timer state transition (start, pause, stop) |

## Usage

Commands are typically issued by game-state listeners or systems when a timer needs to change state:

```cpp
auto context = TimerControlContext{TimerState::Running, myTimerId};

updateContext.queueCommand<TimerControlCommand>(std::move(context));
```


---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::timing::commands
@brief Timer control commands for the command buffer.
@details Contains command classes for requesting timer state transitions through the engine's command system.
</p></details>

