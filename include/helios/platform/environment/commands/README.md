# helios::platform::environment::commands

Commands for platform runtime lifecycle control.

## Commands

| Command | Purpose |
|---------|---------|
| `PlatformInitCommand<THandle>` | Requests runtime/platform initialization |
| `PollEventsCommand<THandle>` | Requests platform event polling |
| `ShutdownCommand<THandle>` | Requests controlled platform shutdown |

## Notes

These commands are usually submitted via command buffers and consumed by
runtime platform systems/managers.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::environment::commands
@brief Commands for platform runtime lifecycle control.
</p></details>

