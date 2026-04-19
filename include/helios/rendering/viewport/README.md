# helios::rendering::viewport

Viewport abstractions and state-based viewport management for render target regions.

## Overview

This module defines normalized viewport bounds, snapshot transport, ECS entity
aliases, and a policy-driven system for determining which viewports should be
rendered based on the current game and match state. This enables different
viewports to be shown during different game phases (e.g., menu viewport during
pause, game viewport during play).

## Key Types

| Item | Description |
|------|-------------|
| `Viewport` | Normalized render region with clear settings and camera binding |
| `ViewportSnapshot` | Immutable per-frame viewport state |
| `ViewportEntityManager` | ECS manager alias for viewport entities |

## Submodules

| Directory | Purpose |
|-----------|---------|
| `components/` | State components for viewport management (`ActiveViewportHandlesStateComponent`) |
| `systems/` | Systems that update viewport state (`StateToViewportPolicyUpdateSystem`) |
| `types/` | Strong ids, handle aliases (`ViewportId`, `ViewportHandle`) |

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
import helios.rendering.viewport;

using namespace helios::rendering::viewport::types;

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
| `helios.gameplay.gamestate` | Game state management |
| `helios.gameplay.matchstate` | Match state management |
| `helios.runtime.world.Session` | Session state storage |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::viewport
@brief Viewport abstractions and state-based viewport management for render target regions.
@details Provides viewport runtime objects, snapshots, entity-manager aliases, and a policy-driven system for determining which viewports should be rendered based on the current game and match state. StateToViewportPolicy maps state combinations to viewport lists, and StateToViewportPolicyUpdateSystem updates the session each frame.
</p></details>
