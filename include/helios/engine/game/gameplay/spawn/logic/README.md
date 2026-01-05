# helios::engine::game::gameplay::spawn::logic

Spawn conditions and strategies for controlling entity creation.

- **SpawnCondition**: Abstract interface for defining when spawning should occur.
- **TimerSpawnCondition**: Time-based spawn condition with configurable intervals.
- **SpawnStrategy**: Abstract interface for defining how spawned entities are configured.
- **CallbackSpawnStrategy**: Strategy that delegates entity configuration to a user-provided callback.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::gameplay::spawn::logic
@brief Spawn conditions and strategies for controlling entity creation.
@details This namespace provides the policy interfaces and implementations for the spawn system. Conditions determine timing, while strategies handle entity initialization.
</p></details>

