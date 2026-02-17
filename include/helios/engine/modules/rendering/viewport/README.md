# helios::engine::modules::rendering::viewport

State-based viewport management for conditional rendering.

## Overview

This module provides a policy-driven system for determining which viewports should be rendered based on the current game and match state. This enables different viewports to be shown during different game phases (e.g., menu viewport during pause, game viewport during play).

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | State components for viewport management |
| `systems/` | Systems that update viewport state |
| `types/` | Policy and configuration types |

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                 StateToViewportPolicy                       │
│  ┌─────────────────────────────────────────────────────────┐│
│  │ GameState::Running  → [gameViewportId]                  ││
│  │ GameState::Paused   → [menuViewportId]                  ││
│  │ MatchState::Playing → [hudViewportId]                   ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│           StateToViewportPolicyUpdateSystem                 │
│  • Reads current GameState/MatchState from Session          │
│  • Queries policy for active viewports                      │
│  • Updates Session with active viewport IDs                 │
└─────────────────────────────────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│                      Rendering                              │
│  • Renders only viewports in active list                    │
└─────────────────────────────────────────────────────────────┘
```

## Usage

```cpp
import helios.engine.modules.rendering.viewport;

using namespace helios::engine::modules::rendering::viewport::types;

// Configure viewport policy
StateToViewportPolicy policy;
policy.add(GameState::Running, gameViewportId)
      .add(GameState::Paused, menuViewportId)
      .add(GameState::GameOver, gameOverViewportId)
      .add(MatchState::Playing, hudViewportId);

// Register system
systemRegistry.add<StateToViewportPolicyUpdateSystem>(
    Phase::Pre, 
    std::move(policy)
);
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.rendering.Viewport` | Low-level viewport definition |
| `helios.engine.mechanics.gamestate` | Game state management |
| `helios.engine.mechanics.match` | Match state management |
| `helios.engine.runtime.world.Session` | Session state storage |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::rendering::viewport
@brief State-based viewport management for conditional rendering.
@details Provides a policy-driven system for determining which viewports should be rendered based on the current game and match state. StateToViewportPolicy maps state combinations to viewport lists, and StateToViewportPolicyUpdateSystem updates the session each frame.
</p></details>

