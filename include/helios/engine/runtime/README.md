# helios::engine::runtime

Runtime infrastructure for game execution, world management, and lifecycle orchestration.

## Overview

This module provides the core runtime components that manage game execution. It includes world state management, messaging infrastructure for commands and events, object pooling for efficient memory reuse, and the game loop orchestration system.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `gameloop/` | Phase-based game loop with configurable passes |
| `world/` | GameWorld, Level, UpdateContext, and system registration |
| `messaging/` | Command buffer, dispatchers, and event buses |
| `pooling/` | GameObject pools and request handlers for object recycling |
| `factory/` | GameObject creation from prefab templates |
| `api/` | (Planned) Stable runtime entry points |

## Architecture

```
runtime/
├── gameloop/      # Game loop orchestration (Pre/Main/Post phases)
├── world/         # GameWorld, Level, UpdateContext
├── messaging/     # Commands + Events (buffer, dispatch, bus)
├── pooling/       # GameObject pools and request handlers
├── factory/       # GameObjectFactory for prefab instantiation
└── api/           # (Future) Stable entry points
```

## Key Concepts

**GameWorld** owns all GameObjects and provides query methods for component-based iteration. It manages the lifecycle of entities and coordinates with pools and managers.

**GameLoop** orchestrates the frame update cycle through three phases (Pre, Main, Post), each containing passes that execute systems in order.

**CommandBuffer** collects commands during a phase and flushes them to dispatchers after each phase completes.

**GameLoopEventBus** manages phase-level and pass-level event propagation using a double-buffer pattern.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime
@brief Runtime infrastructure for game execution and lifecycle orchestration.
@details Provides the core runtime components including GameWorld, GameLoop, messaging (commands/events), and object pooling systems.
</p></details>

