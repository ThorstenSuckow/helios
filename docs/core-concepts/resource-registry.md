# Resource Registry

The ResourceRegistry is the central **type-indexed service locator** for all engine-level resources. It provides O(1) access to Managers and CommandBuffers.

## Overview

During startup, the application registers Managers (e.g., `SpawnManager`, `GameObjectPoolManager`, `ScorePoolManager`) and CommandBuffers (e.g., `EngineCommandBuffer`) with the ResourceRegistry. At runtime, systems look up resources by type in O(1) via a flat `void*` array indexed by `ResourceTypeId`.

```
┌─────────────────────────────────────────────────────────────────────┐
│                     RESOURCE REGISTRY                               │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  REGISTRATION (startup)                                             │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │  gameWorld.registerManager<GameObjectPoolManager>()           │  │
│  │  gameWorld.registerManager<SpawnManager>()                    │  │
│  │  gameWorld.registerManager<ScorePoolManager>()                │  │
│  │  gameWorld.registerCommandBuffer<EngineCommandBuffer>()       │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
│  STORAGE (dual-registry)                                            │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │  managerRegistry_       ConceptModelRegistry<Manager>         │  │
│  │  commandBufferRegistry_ ConceptModelRegistry<CommandBuffer>   │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
│  LOOKUP (hot path, O(1) via type index)                             │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │  auto* mgr = registry.resource<SpawnManager>();               │  │
│  │  // → managerRegistry_.item<SpawnManager>()                   │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Key Concepts

### Type-Indexed O(1) Access

Each resource type is assigned a unique, monotonically increasing ID at compile time. Managers use `ResourceTypeId`, CommandBuffers use `CommandBufferTypeId`. Both are backed by `TypeIndexer`. The IDs serve as indices into the underlying `ConceptModelRegistry`'s fast-access array:

```cpp
// ResourceTypeId assigns a compile-time index to each type
template <typename T>
static ResourceTypeId id() {
    static const size_t tid = TypeIndexer<ResourceType>::typeIndex<T>();
    return ResourceTypeId(tid);
}

// ResourceRegistry routes to the correct sub-registry
template<class T>
T* resource() noexcept {
    if constexpr (IsManagerLike<T>) {
        return managerRegistry_.item<T>();  // O(1) via type index
    }
    if constexpr (IsCommandBufferLike<T>) {
        return commandBufferRegistry_.item<T>();  // O(1) via type index
    }
    return nullptr;
}
```

No hash computation, no virtual dispatch — just an array index within the respective registry.

### Ownership

Resources are owned through two parallel registries based on the Concept/Model pattern:

- **Managers** are owned by the `ManagerRegistry` (`ConceptModelRegistry<Manager, ResourceTypeId>`), which wraps each concrete manager in a type-erased `Manager` value.
- **CommandBuffers** are owned by the `CommandBufferRegistry` (`ConceptModelRegistry<CommandBuffer, CommandBufferTypeId>`), which wraps each concrete buffer in a type-erased `CommandBuffer` value.

Both registries provide O(1) type-based lookup and insertion-order iteration.

### Dual-Registry Pattern

Resources are stored in two typed registries, each optimized for its domain:

| Registry | Purpose | Access Pattern |
|----------|---------|----------------|
| `managerRegistry_` | Type-erased Manager storage | `init()`, `flushManagers()`, `reset()` |
| `commandBufferRegistry_` | Type-erased CommandBuffer storage | `flush()` at commit points |

GameWorld provides domain-specific convenience methods that delegate to `ResourceRegistry::emplace<T>()`:

### Two Registration Modes

#### 1. Owning Registration (Managers, CommandBuffers)

Managers are registered via `registerManager<T>()`:

```cpp
auto& spawnMgr = gameWorld.registerManager<SpawnManager>(args...);
// → delegates to ResourceRegistry::emplace<T>() (IsManagerLike overload)
// → stored in managerRegistry_ (ConceptModelRegistry<Manager>)
```

CommandBuffers are registered via `registerCommandBuffer<T>()`:

```cpp
auto& cmdBuf = gameWorld.registerCommandBuffer<EngineCommandBuffer>();
// → delegates to ResourceRegistry::emplace<T>() (IsCommandBufferLike overload)
// → stored in commandBufferRegistry_ (ConceptModelRegistry<CommandBuffer>)
```

#### 2. Non-Owning Registration (CommandHandlers)

Command handlers are registered via `GameWorld::registerCommandHandler<Cmd>(owner)` which delegates to the `CommandHandlerRegistry` (stored on the GameWorld). Handlers are not stored in the ResourceRegistry itself.

## Integration with GameWorld

The ResourceRegistry is a member of `GameWorld` and accessed via `gameWorld.resourceRegistry()`. Note that `UpdateContext` does **not** expose the ResourceRegistry — systems interact with it indirectly through `queueCommand<T>()` and other typed accessors.

```cpp
// Convenience methods on GameWorld delegate to the registry
auto& poolMgr  = gameWorld.registerManager<GameObjectPoolManager>();
auto& spawnMgr = gameWorld.registerManager<SpawnManager>();
auto& cmdBuf   = gameWorld.registerCommandBuffer<EngineCommandBuffer>();

// Typed access
auto& mgr = gameWorld.manager<SpawnManager>();    // checked (asserts)
auto* mgr = gameWorld.tryManager<SpawnManager>(); // returns nullptr if missing
```

### Initialization Sequence

1. **Registration:** Managers are registered via `registerManager<T>()`, CommandBuffers via `registerCommandBuffer<T>()`.
2. **init():** `GameWorld::init()` iterates `managerRegistry_.items()` and calls `init(gameWorld)` on each. Managers use `init()` to register their CommandHandlers.
3. **Runtime:** Systems submit commands via `UpdateContext::queueCommand<T>()`. The `TypedCommandBuffer` resolves handlers internally through the `CommandHandlerRegistry`.
4. **Flush:** At each commit point, `flushCommandBuffers(ctx)` routes commands, then `flushManagers(ctx)` calls `flush(updateContext)` on each manager.
5. **Reset:** `GameWorld::reset()` iterates `managerRegistry_.items()` and calls `reset()` for level transitions.

```
Registration     →  init()  →  Runtime Loop        →  reset()
    │                  │            │                      │
    ▼                  ▼            ▼                      ▼
registerManager    mgr.init()  queueCommand<T>()     mgr.reset()
registerCmdBuffer  (registers   (routed internally
                    handlers)    via CommandHandlerRegistry)
```

## Integration with Command System

The TypedCommandBuffer uses the `CommandHandlerRegistry` to route commands to handlers at flush time:

```cpp
// During TypedCommandBuffer::flush():
if (registry.has<Cmd>()) {
    auto& handlerRef = registry.tryHandler<Cmd>();
    for (auto& cmd : queue) {
        handlerRef.submit(cmd);
    }
} else if constexpr (ExecutableCommand<Cmd>) {
    for (auto& cmd : queue) {
        cmd.execute(updateContext);
    }
}
```

A Manager registers itself as handler during `init()`:

```cpp
void SpawnManager::init(GameWorld& gameWorld) {
    gameWorld.registerCommandHandler<SpawnCommand>(*this);
    gameWorld.registerCommandHandler<DespawnCommand>(*this);
}
```

## API Summary

### GameWorld Convenience Methods

| Method | Description |
|--------|-------------|
| `registerManager<T>(args...)` | Registers and constructs a Manager |
| `registerCommandBuffer<T>(args...)` | Registers and constructs a CommandBuffer |
| `manager<T>()` | Returns a reference (asserts if not registered) |
| `tryManager<T>()` | Returns a pointer, or nullptr if not registered |
| `tryCommandBuffer<T>()` | Returns a pointer, or nullptr if not registered |
| `hasManager<T>()` | Checks if a Manager is registered |
| `hasCommandBuffer<T>()` | Checks if a CommandBuffer is registered |
| `registerCommandHandler<Cmd>(owner)` | Registers a non-owning CommandHandler reference |

### ResourceRegistry Methods

| Method | Description |
|--------|-------------|
| `emplace<T>(args...)` | Creates and registers a resource (Manager or CommandBuffer) |
| `get<T>()` | Returns a reference (asserts if not registered) |
| `tryGet<T>()` | Returns a pointer, or nullptr if not registered |
| `has<T>()` | Checks if a resource is registered |
| `managers()` | Returns a span of all registered Managers |
| `commandBuffers()` | Returns a span of all registered CommandBuffers |

## Extending the Registry

The ResourceRegistry is designed for extensibility. Applications register their own Managers and CommandBuffers alongside the engine-provided ones.

### Custom Managers

A custom Manager is a plain class that provides `flush(UpdateContext&)`, declares `using EngineRoleTag = ManagerRole;`, and optionally implements `init(GameWorld&)` and `reset()`. If the Manager also handles commands, it provides a `submit(const Cmd&)` method for each command type and registers these in `init()`.

#### Step 1: Define the Command (if needed)

```cpp
export module myapp.commands;

import helios.engine.runtime.world.UpdateContext;

export namespace myapp::commands {

    struct CoinPickupCommand {
        EntityHandle collector;
        int value;
    };
}
```

#### Step 2: Implement the Manager

```cpp
export module myapp.CoinManager;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.common.tags.ManagerRole;
import myapp.commands;

export namespace myapp {

    using namespace helios::engine::runtime::world;
    using namespace helios::engine::runtime::messaging::command;

    class CoinManager {

        std::vector<commands::CoinPickupCommand> pending_;

    public:

        using EngineRoleTag = helios::engine::common::tags::ManagerRole;

        // Called by TypedCommandBuffer during flush
        bool submit(commands::CoinPickupCommand cmd) noexcept {
            pending_.push_back(cmd);
            return true;
        }

        // Register this handler so the TypedCommandBuffer can route to it
        void init(GameWorld& gameWorld) {
            gameWorld.registerCommandHandler<commands::CoinPickupCommand>(*this);
        }

        // Process queued commands (called after CommandBuffer::flush)
        void flush(UpdateContext& ctx) noexcept {
            for (const auto& cmd : pending_) {
                // award coins, update score, etc.
            }
            pending_.clear();
        }

        void reset() {
            pending_.clear();
        }
    };
}
```

#### Step 3: Register with the GameWorld

```cpp
auto& coinMgr = gameWorld.registerManager<myapp::CoinManager>();

// After all resources are registered:
gameWorld.init(); // Calls CoinManager::init() → registers handler
```

At runtime, systems submit engine-defined commands via `ctx.queueCommand<T>()`, which internally routes to the `EngineCommandBuffer`. For custom commands not covered by the engine buffer, the logic would reside in a custom command buffer or a specific handler lookup (though usually handled via `EngineCommandBuffer` if type is added to it, otherwise via specific buffer).
