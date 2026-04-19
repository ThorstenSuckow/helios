# helios::gameplay::scoring::systems

Systems for score processing and observation.

This namespace contains systems that handle scoring events, update score and high score observers, and manage observer state lifecycle.

## Systems

| System | Phase | Description |
|--------|-------|-------------|
| `CombatScoringSystem` | Update | Processes death events and issues score commands |
| `ScoreObserverSystem` | Update | Propagates score snapshots from ScorePools to ScoreObserverComponents |
| `MaxScoreObserverSystem` | Update | Propagates high score snapshots from ScorePools to MaxScoreObserverComponents |
| `ScoreObserverClearSystem` | Post | Resets hasUpdate flags on ScoreObserverComponents |
| `MaxScoreObserverClearSystem` | Post | Resets hasUpdate flags on MaxScoreObserverComponents |

## Execution Order

1. **CombatScoringSystem** - Listens for `HealthDepletedEvent`, checks for `ScoreValueComponent`, issues `UpdateScoreCommand`
2. **ScoreObserverSystem** - Compares revision, propagates `ScorePoolSnapshot` to `ScoreObserverComponent`
3. **MaxScoreObserverSystem** - Compares revision, propagates `MaxScorePoolSnapshot` to `MaxScoreObserverComponent`
4. **ScoreObserverClearSystem** - Clears `hasUpdate` flag after UI systems have processed score changes
5. **MaxScoreObserverClearSystem** - Clears `hasUpdate` flag after UI systems have processed high score changes

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::scoring::systems
@brief Systems for score processing and observation.
@details Contains systems for combat-based scoring, score pool observation (both current and high score), and observer state management.
</p></details>
