# helios::engine::mechanics::scoring::commands

Score update commands for the command buffer.

This namespace contains command classes used to request score updates through the command system.

## Commands

| Command | Description |
|---------|-------------|
| `UpdateScoreCommand` | Command that carries score update information to the ScorePoolManager |

## Usage

Commands are typically issued by scoring systems (e.g., `CombatScoringSystem`) when a scoring event occurs:

```cpp
auto scoreContext = types::ScoreValueContext{
    .scoreTypeId = ScoreTypeId::id<KillReward>(),
    .scorePoolId = playerScorePoolId,
    .value = 100.0
};

updateContext.commandBuffer().add<UpdateScoreCommand>(std::move(scoreValueContext));
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::scoring::commands
@brief Score update commands for the command buffer.
@details Contains command classes for requesting score updates through the engine's command system.
</p></details>
