# helios::engine::mechanics::scoring::systems

Systems for score processing and observation.

This namespace contains systems that handle scoring events, update score observers, and manage score state lifecycle.

## Systems

| System | Phase | Description |
|--------|-------|-------------|
| `CombatScoringSystem` | Update | Processes death events and issues score commands |
| `ScoreObserverSystem` | Update | Syncs ScoreObserverComponent values from ScorePools |
| `ScoreObserverClearSystem` | Post | Clears hasUpdate flags on ScoreObserverComponents |

## Execution Order

1. **CombatScoringSystem** - Listens for `DeathEvent`, checks for `ScoreValueComponent`, issues `UpdateScoreCommand`
2. **ScoreObserverSystem** - Reads from `ScorePool` and updates `ScoreObserverComponent.value`
3. **ScoreObserverClearSystem** - Clears `hasUpdate` flag after UI systems have processed changes

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::scoring::systems
@brief Systems for score processing and observation.
@details Contains systems for combat-based scoring, score pool observation, and observer state management.
</p></details>
