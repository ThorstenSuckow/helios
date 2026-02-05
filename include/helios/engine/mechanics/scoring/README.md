# helios::engine::mechanics::scoring

Score management and tracking system for game mechanics.

This module provides components, systems, and infrastructure for tracking and managing scores in the helios engine. It supports multiple score pools (e.g., per-player), score types (e.g., kill rewards), and integrates with the combat and UI systems.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `commands/` | Score update commands for the command buffer |
| `components/` | Score-related ECS components |
| `systems/` | Systems for score processing and observation |
| `types/` | Score data types and value classes |

## Key Classes

| Class | Purpose |
|-------|---------|
| `ScorePoolManager` | Manager that owns score pools and handles score commands |
| `ScorePool` | Container accumulating scores by type with running total |
| `ScorePoolSnapshot` | Immutable snapshot of pool state |
| `ScoreCommandHandler` | Abstract interface for score command processing |
| `ScoreCommandDispatcher` | Routes score commands to the handler |

## Architecture

The scoring system follows a command-based architecture:

1. **Combat events** trigger `UpdateScoreCommand` via `CombatScoringSystem`
2. **Commands** are dispatched to `ScorePoolManager` via `ScoreCommandDispatcher`
3. **Scores** are accumulated in `ScorePool` instances
4. **UI** observes pools via `ScoreObserverComponent` + `ScoreObserverSystem`

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::scoring
@brief Score management and tracking system for game mechanics.
@details Provides infrastructure for tracking scores across multiple pools, processing score commands, and exposing score data to UI systems.
</p></details>
