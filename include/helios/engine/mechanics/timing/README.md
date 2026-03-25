# helios::engine::mechanics::timing

Game timer management system.

This module provides timers, commands, components, and systems for tracking elapsed time, controlling timer state transitions, and observing timer updates from within the ECS.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `types/` | Core type definitions (`TimerState`, `TimerRevision`, `TimerControlContext`) |
| `commands/` | Command classes for requesting timer state transitions |
| `components/` | ECS components for observing timers |
| `systems/` | Systems that advance timers each frame |

## Key Classes

| Class | Description |
|-------|-------------|
| `GameTimer` | A named timer that tracks elapsed time while in the Running state |
| `TimerManager` | Manager that owns timers and processes pending control commands during flush |
| `TimerCommandHandler` | Abstract handler interface for timer control commands |

## Architecture

1. **GameTimer** accumulates elapsed time and increments a revision counter on each update
2. **TimerControlCommand** is written into the command buffer to request state transitions (start, pause, stop)
3. **TimerCommandDispatcher** routes the command to the **TimerManager** via the Visitor pattern
4. **TimerManager** collects pending commands and applies them during `flush()`
5. **GameTimerUpdateSystem** advances all timers by the current delta time each frame
6. **GameTimerBindingComponent** allows entities to track a specific timer's revision

## Integration

The timing system integrates with:
- **Command system** - timer state transitions are requested via `TimerControlCommand`
- **Manager system** - `TimerManager` participates in the game loop flush cycle
- **Game state listeners** - can issue timer commands in response to state changes

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::timing
@brief Game timer management system.
@details Provides timers, commands, components and systems for tracking elapsed time and controlling timer state transitions through the engine's command system.
</p></details>

