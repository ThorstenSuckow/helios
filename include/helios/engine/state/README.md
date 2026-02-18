# helios::engine::state

State-to-ID mapping utilities for policy-driven viewport activation.

## Overview

This module provides generic mapping utilities that associate state enum values with lists of IDs. The primary use case is determining which viewports should be active based on the current game and match state.

## Components

| Class | Description |
|-------|-------------|
| `StateToIdMap<TState, TId>` | Maps a single state type to ID lists |
| `StateToIdMapPair<LState, RState, TId>` | Combines two state maps, returns union at lookup |
| `CombinedStateToIdMapPair<LState, RState, TId>` | Maps state pairs directly to ID lists |

## StateToIdMapPair vs CombinedStateToIdMapPair

- **StateToIdMapPair**: Associates IDs with individual states (left or right). At lookup, returns the sorted union of both states' ID lists. Use when IDs should appear for a state regardless of the other state.

- **CombinedStateToIdMapPair**: Associates IDs with specific state combinations. Uses a 2D matrix indexed by both states. Use when IDs should only appear for exact state pairs. Supports fallback to the left state when the right state has no registered IDs.

## Usage

### StateToIdMapPair (Union Semantics)

```cpp
using namespace helios::engine::state;
using namespace helios::engine::core::data;

StateToIdMapPair<GameState, MatchState, ViewportId> policy;

// IDs associated with individual states
policy.add(GameState::Running, ViewportId("game"));
policy.add(GameState::Paused, ViewportId("pause_menu"));
policy.add(MatchState::GameOver, ViewportId("game_over"));

policy.freeze();

// Returns union: {"game", "game_over"} if both states match
auto ids = policy.ids(GameState::Running, MatchState::GameOver);
```

### CombinedStateToIdMapPair (Exact Combination)

```cpp
CombinedStateToIdMapPair<GameState, MatchState, ViewportId> policy;

// IDs for specific state combinations
policy.add(GameState::Running, MatchState::Playing, ViewportId("game"));
policy.add(GameState::Running, MatchState::GameOver, ViewportId("game_over"));
policy.add(GameState::Paused, MatchState::Undefined, ViewportId("pause_menu"));

policy.freeze();

// Returns only IDs for the exact combination
auto ids = policy.ids(GameState::Running, MatchState::Playing);
```

## Design

- **Bit-indexed storage:** States must be power-of-two values. The map uses bit position as the index for O(1) lookup.
- **Freeze pattern:** Maps support a `freeze()` operation that finalizes, sorts, and shrinks internal storage.
- **Union semantics (StateToIdMapPair):** Returns the sorted union of IDs from both maps.
- **Fallback semantics (CombinedStateToIdMapPair):** Falls back to index 0 (undefined right state) when specific combination has no IDs.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state
@brief State-to-ID mapping utilities.
@details Provides generic mapping templates for associating state enum values with lists of IDs. Used by StateToViewportPolicyUpdateSystem to determine active viewports based on game/match state.
</p></details>

