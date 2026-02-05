# helios::engine::mechanics::scoring::types

Score data types and value classes.

This namespace contains types for score values, score contexts, and related data structures used throughout the scoring system.

## Types

| Type | Description |
|------|-------------|
| `Score` | Abstract base class for score value types |
| `KillReward` | Concrete score type for kill rewards |
| `ScoreContext` | Data structure containing score type, pool ID, and value |
| `ScorePoolRevision` | Type alias for tracking pool changes |

## Score Hierarchy

```
Score (abstract)
└── KillReward
└── (custom score types...)
```

## ScoreContext

Used to transfer score data through the command system:

```cpp
ScoreValueContext{
    .scoreTypeId = ScoreTypeId::id<KillReward>(),
    .scorePoolId = ScorePoolId{"player1"},
    .value = 100.0
};
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::scoring::types
@brief Score data types and value classes.
@details Contains the Score base class, concrete score types like KillReward, and supporting data structures like ScoreContext.
</p></details>
