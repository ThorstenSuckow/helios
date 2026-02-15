# helios::engine::mechanics::gamestate::commands

Commands for requesting game state transitions.

## Commands

| Command | Description |
|---------|-------------|
| `GameStateCommand` | Encapsulates a state transition request |

## GameStateCommand

A world command that carries a `GameStateTransitionRequest`. Commands are submitted to the `CommandBuffer` and routed via dispatchers to the `GameStateManager`.

Direct execution via `execute()` is a no-op; the command must be dispatched.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate::commands
@brief Commands for requesting game state transitions.
@details Contains world commands for triggering state machine transitions.
</p></details>

