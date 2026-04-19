# helios::platform::environment::systems

Systems executing runtime platform lifecycle logic.

## Systems

| System | Purpose |
|--------|---------|
| `PlatformInitSystem<THandle>` | Handles runtime/platform initialization flow |
| `PollEventsSystem<THandle>` | Triggers per-frame platform event polling |
| `WindowBasedShutdownSystem<THandle>` | Handles shutdown requests |
| `WarmupDoneSystem<THandle>` | Finalizes runtime warmup state transitions |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::environment::systems
@brief Systems for runtime platform lifecycle processing.
</p></details>

