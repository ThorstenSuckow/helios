# helios::engine::runtime::world

Runtime world coordination, registries, and frame update context.

## Overview

This module provides the runtime root API for orchestration of frame updates.
`GameWorld` coordinates typed entity domains through `EngineWorld`, owns runtime
services (`Session`, `RuntimeEnvironment`), and manages typed resource
registries for managers and command buffers.

`UpdateContext` carries frame-scoped data into systems (timing, input, event
bus channels, viewport snapshots, level pointer, and typed command submission).

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameWorld` | Runtime root coordinating worlds, registries, session, and environment |
| `EngineWorld` | Handle-routed aggregate world for game-object/platform/render domains |
| `ResourceRegistry` | Type-indexed storage for managers and command buffers |
| `UpdateContext` | Per-frame context passed to system updates |
| `Session` | Runtime session state facade |
| `RuntimeEnvironment` | Platform/runtime readiness facade |
| `Level` | Active level payload owned by `GameWorld` |

## Submodules

| Directory | Purpose |
|-----------|---------|
| `concepts/` | Runtime-world specific concepts (e.g. game-object handle concept) |
| `types/` | Runtime-world ids/type-index ids/handle aliases |

## Usage

```cpp
helios::engine::runtime::world::GameWorld world;

world.registerCommandBuffer<EngineCommandBuffer>();
world.registerManager<SomeManager>();
world.init();

auto entity = world.add<helios::engine::runtime::world::types::GameObjectHandle>();

// In systems: UpdateContext is used for frame-scoped access
// updateContext.queueCommand<EngineCommandBuffer, SomeCommand>(...);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::world
@brief Runtime world coordination, registries, and per-frame update context.
</p></details>
