# helios::platform::lifecycle::systems

Systems executing platform lifecycle transitions.

## Systems

| System | Purpose |
|--------|---------|
| `PlatformInitSystem<TCommandBuffer>` | Submits platform initialization commands while the session is uninitialized |
| `WarmupDoneSystem<THandle, TCommandBuffer>` | Signals warmup completion once warmup shader sources are consumed |
| `WindowBasedShutdownSystem<THandle, TCommandBuffer>` | Requests shutdown when no active window entities remain |
| `DestroySessionSystem` | Marks the active session as destroyed |

## Notes

`PollEventsSystem` belongs to `helios::platform::environment::systems` and is
executed as part of regular frame service, not lifecycle transition handling.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::lifecycle::systems
@brief Systems for platform lifecycle transition processing.
</p></details>
