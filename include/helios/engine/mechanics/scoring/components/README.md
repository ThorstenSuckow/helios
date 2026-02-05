# helios::engine::mechanics::scoring::components

Score-related ECS components.

This namespace contains components for associating entities with score pools, storing score values, and observing score changes.

## Components

| Component | Description |
|-----------|-------------|
| `ScorePoolComponent` | Associates an entity with a score pool (e.g., player → player's pool) |
| `ScoreValueComponent<T>` | Template component storing a score value of type T (e.g., KillReward) |
| `ScoreObserverComponent` | Observes a score pool and caches its value for UI display |

## Usage

### Attaching score pool to player

```cpp
auto& poolComp = player.add<ScorePoolComponent>();
poolComp.setScorePoolId(ScorePoolId{"player1"});
```

### Defining kill reward on enemy

```cpp
auto& scoreValue = enemy.add<ScoreValueComponent<KillReward>>(100.0);
```

### Observing score for UI

```cpp
auto& observer = scoreText.add<ScoreObserverComponent>();
observer.setScorePoolId(ScorePoolId{"player1"});
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::scoring::components
@brief Score-related ECS components.
@details Contains components for score pool association, score value storage, and score observation for UI binding.
</p></details>
