# helios::engine::mechanics::match::commands

Commands for requesting match state transitions.

## Commands

| Command | Description |
|---------|-------------|
| `MatchStateCommand` | Encapsulates a match state transition request |

## MatchStateCommand

A world command that carries a `MatchStateTransitionRequest`. Commands are submitted to the `CommandBuffer` and routed via dispatchers to the `MatchStateManager`.

Direct execution via `execute()` is a no-op; the command must be dispatched.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::match::commands
@brief Commands for requesting match state transitions.
@details Contains world commands for triggering state machine transitions.
</p></details>

