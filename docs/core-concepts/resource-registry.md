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
│  │  gameWorld.resourceRegistry()                                 │  │
│  │      .registerResource<EngineCommandBuffer>()                 │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
│  STORAGE                                                            │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │  owned_           [ErasedUnique, ...]             (ownership) │  │
│  │  fastAccess_      [void*, void*, ...]            (O(1) lookup)│  │
│  │  managerRegistry_ ConceptModelRegistry<Manager>   (iteration) │  │
│  │  cmdBuffers_      [CommandBuffer*, ...]           (iteration) │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
│  LOOKUP (hot path)                                                  │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │  auto& mgr = registry.resource<SpawnManager>();               │  │
│  │  // → fastAccess_[ResourceTypeId::id<SpawnManager>().value()] │  │
│  │  // → static_cast<SpawnManager*>(ptr)                         │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Key Concepts

### Type-Indexed O(1) Access

Each resource type is assigned a unique, monotonically increasing ID via `ResourceTypeId` (backed by `TypeIndexer`). This ID serves as an index into the `fastAccess_` vector:

```cpp
// ResourceTypeId assigns a compile-time index to each type
template <typename T>
static ResourceTypeId id() {
    static const size_t tid = TypeIndexer<ResourceType>::typeIndex<T>();
    return ResourceTypeId(tid);
}

// Lookup is a single array index + static_cast
T& resource() noexcept {
    const size_t idx = ResourceTypeId::id<T>().value();
    return *static_cast<T*>(fastAccess_[idx]);
}
```

No hash computation, no virtual dispatch — just an array index.

### Ownership

Resources are owned through two mechanisms:

- **Managers** are owned by the `ManagerRegistry` (`ConceptModelRegistry<Manager, ResourceTypeId>`), which wraps each concrete manager in a type-erased `Manager` value and stores it in a dense vector.
- **CommandBuffers and other resources** are heap-allocated and owned through `ErasedUnique`, a type-erased unique ownership wrapper (16 bytes: `void*` + function pointer deleter):

```cpp
struct ErasedUnique {
    void* ptr = nullptr;
    void (*destroy)(void*) noexcept = nullptr;
};
```

The deleter captures the concrete type at registration time and invokes `delete static_cast<T*>(ptr)` on destruction.

### Dual-Access Pattern

Resources are stored in parallel data structures, optimized for different access patterns:

| Structure | Purpose | Access Pattern |
|-----------|---------|----------------|
| `fastAccess_` | O(1) type-based lookup | Hot path: every system, every frame |
| `managerRegistry_` | Type-erased Manager storage (`ConceptModelRegistry`) | `init()`, `flushManagers()`, `reset()` |
| `commandBuffers_` | Linear iteration over CommandBuffers | `flush()` at commit points |
| `owned_` | Lifetime management | Destruction only |

The same raw pointer appears in multiple vectors. This trades a few extra bytes of redundant pointers for zero-cost access in each use case.

### Two Registration Modes

#### 1. Owning Registration (Managers, CommandBuffers)

Managers are registered via the `IsManager<T>` overload:

```cpp
auto& spawnMgr = gameWorld.registerManager<SpawnManager>(args...);
// → delegates to ResourceRegistry::registerResource<T>() (IsManager overload)
// → stored in managerRegistry_ (ConceptModelRegistry<Manager>)
// → indexed in fastAccess_
```

CommandBuffers and other non-Manager resources use the general overload:

```cpp
auto& cmdBuf = registry.registerResource<EngineCommandBuffer>();
// → heap-allocated via std::make_unique, owned by ErasedUnique in owned_
// → indexed in fastAccess_ + appended to commandBuffers_ if CommandBuffer-derived
```

#### 2. Non-Owning Registration (CommandHandlers)

Command handlers are registered via `GameWorld::registerCommandHandler<Cmd>(owner)` which delegates to the `CommandHandlerRegistry` (stored on the GameWorld). Handlers are not stored in the ResourceRegistry itself.

## Integration with GameWorld

The ResourceRegistry is a member of `GameWorld` and accessed via `gameWorld.resourceRegistry()`. Note that `UpdateContext` does **not** expose the ResourceRegistry — systems interact with it indirectly through `queueCommand<T>()` and other typed accessors.

```cpp
// Direct registry access (initialization time only)
auto& poolMgr = gameWorld.resourceRegistry()
    .registerResource<GameObjectPoolManager>();

// Convenience methods on GameWorld delegate to the registry
auto& spawnMgr = gameWorld.registerManager<SpawnManager>();
T& mgr = gameWorld.manager<SpawnManager>();
```

### Initialization Sequence

1. **Registration:** Resources are registered via `registerResource()` or `registerManager()`.
2. **init():** `GameWorld::init()` iterates `managerRegistry_.items()` and calls `init(gameWorld)` on each. Managers use `init()` to register their CommandHandlers.
3. **Runtime:** Systems submit commands via `UpdateContext::queueCommand<T>()`. The `TypedCommandBuffer` resolves handlers internally through the `ResourceRegistry`.
4. **Flush:** At each commit point, `commandBuffer.flush(gameWorld, ctx)` routes commands, then `flushManagers(ctx)` calls `flush(updateContext)` on each manager.
5. **Reset:** `GameWorld::reset()` iterates `managerRegistry_.items()` and calls `reset()` for level transitions.

```
Registration  →  init()  →  Runtime Loop        →  reset()
    │               │            │                      │
    ▼               ▼            ▼                      ▼
registerResource  mgr.init()  queueCommand<T>()     mgr.reset()
                  (registers   (routed internally
                   handlers)    via ResourceRegistry)
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

| Method | Description |
|--------|-------------|
| `registerResource<T>(args...)` | Creates and registers an owning resource |
| `registerResource<T>(ref)` | Registers a non-owning CommandHandler reference |
| `resource<T>()` | Returns a reference (asserts if not registered) |
| `tryResource<T>()` | Returns a pointer, or nullptr if not registered |
| `has<T>()` | Checks if a resource is registered |
| `managers()` | Returns a span of all registered Managers |
| `commandBuffers()` | Returns a span of all registered CommandBuffers |

## Extending the Registry

The ResourceRegistry is designed for extensibility. Applications register their own Managers and CommandBuffers alongside the engine-provided ones.

### Custom Managers

A custom Manager is a plain class that provides `flush(UpdateContext&)`, declares `using EngineRoleTag = ManagerTag;`, and optionally implements `init(GameWorld&)` and `reset()`. If the Manager also handles commands, it provides a `submit(const Cmd&)` method for each command type and registers these in `init()`.

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
import helios.engine.common.tags.ManagerTag;
import myapp.commands;

export namespace myapp {

    using namespace helios::engine::runtime::world;
    using namespace helios::engine::runtime::messaging::command;

    class CoinManager {

        std::vector<commands::CoinPickupCommand> pending_;

    public:

        using EngineRoleTag = helios::engine::common::tags::ManagerTag;

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
