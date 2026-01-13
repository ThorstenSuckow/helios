# helios::engine::game::gameplay::spawn::commands

Commands for spawn and despawn operations.

This namespace contains command classes that represent spawn and despawn requests. These commands are emitted by systems and processed by dispatchers during the command flush phase.

## Commands

| Command | Purpose |
|---------|---------|
| `SpawnCommand` | Requests spawning of GameObjects from a pool |
| `DespawnCommand` | Requests despawning and pool return of a GameObject |

## Architecture

Commands follow the visitor pattern with dispatchers:

```
System → CommandBuffer → Dispatcher → Manager
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::gameplay::spawn::commands
@brief Commands for spawn and despawn operations.
@details This namespace contains command classes for deferred spawn and despawn operations. Commands are processed during the command buffer flush phase.
</p></details>

