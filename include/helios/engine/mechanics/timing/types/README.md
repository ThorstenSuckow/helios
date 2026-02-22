# helios::engine::mechanics::timing::types

Core type definitions for the timing module.

## Types

| Type | Description |
|------|-------------|
| `TimerState` | Enumeration of timer states: `Undefined`, `Started`, `Paused`, `Stopped` |
| `TimerRevision` | Monotonically increasing revision counter (`uint32_t`) |
| `TimerControlContext` | Context struct carrying a target `TimerState` and `GameTimerId` |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::timing::types
@brief Core type definitions for the timing module.
@details Contains state enums, revision counters, and context structs used across the timing subsystem.
</p></details>

