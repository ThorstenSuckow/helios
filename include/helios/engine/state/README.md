# helios::engine::state

State-to-ID mapping utilities for policy-driven viewport activation.

## Overview

This module provides generic mapping utilities that associate state enum values with lists of IDs. The primary use case is determining which viewports should be active based on the current game and match state.

## Components

| Class | Description |
|-------|-------------|
| `StateToIdMap<TState, TId>` | Maps a single state type to ID lists |
| `StateToIdMapPair<LState, RState, TId>` | Combines two state maps for dual-state lookups |

## Usage

```cpp
using namespace helios::engine::state;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::match::types;
using namespace helios::engine::core::data;

// Create a combined map for game and match states
StateToIdMapPair<GameState, MatchState, ViewportId> policy;

// Associate viewports with states
policy.add(GameState::Running, ViewportId("game"));
policy.add(GameState::Paused, ViewportId("pause_menu"));
policy.add(MatchState::GameOver, ViewportId("game_over"));

// Freeze before use
policy.freeze();

// Query IDs for a state combination
auto ids = policy.ids(GameState::Running, MatchState::Playing);
```

## Design

- **Bit-indexed storage:** States must be power-of-two values. The map uses bit position as the index for O(1) lookup.
- **Freeze pattern:** Maps support a `freeze()` operation that finalizes, sorts, and shrinks internal storage.
- **Union semantics:** `StateToIdMapPair::ids()` returns the sorted union of IDs from both maps.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state
@brief State-to-ID mapping utilities.
@details Provides generic mapping templates for associating state enum values with lists of IDs. Used by StateToViewportPolicyUpdateSystem to determine active viewports based on game/match state.
</p></details>

