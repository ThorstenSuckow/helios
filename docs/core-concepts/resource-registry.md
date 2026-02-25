# Resource Registry

The ResourceRegistry is the central **type-indexed service locator** for all engine-level resources. It provides O(1) access to Managers, CommandBuffers, and CommandHandlers, and owns the lifetime of all heap-allocated resources.

## Overview

During startup, the application registers Managers (e.g., `SpawnManager`, `GameObjectPoolManager`, `ScorePoolManager`) and CommandBuffers (e.g., `EngineCommandBuffer`) with the ResourceRegistry. At runtime, systems look up resources by type in O(1) via a flat `void*` array indexed by `ResourceTypeId`.

```
┌─────────────────────────────────────────────────────────────────────┐
│                     RESOURCE REGISTRY                               │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  REGISTRATION (startup)                                             │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │  gameWorld.resourceRegistry()                                 │  │
│  │      .registerResource<GameObjectPoolManager>()               │  │
│  │      .registerResource<SpawnManager>()                        │  │
│  │      .registerResource<ScorePoolManager>()                    │  │
│  │      .registerResource<EngineCommandBuffer>()                 │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
│  STORAGE                                                            │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │  owned_       [ErasedUnique, ErasedUnique, ...]  (ownership)  │  │
│  │  fastAccess_  [void*, void*, void*, ...]         (O(1) lookup)│  │
│  │  managers_    [Manager*, Manager*, ...]           (iteration) │  │
│  │  cmdBuffers_  [CommandBuffer*, ...]               (iteration) │  │
│  │  cmdHandlers_ [CommandHandler*, ...]              (routing)   │  │
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

### Ownership via ErasedUnique

Resources are heap-allocated and owned through `ErasedUnique`, a type-erased unique ownership wrapper (16 bytes: `void*` + function pointer deleter):

```cpp
struct ErasedUnique {
    void* ptr = nullptr;
    void (*destroy)(void*) noexcept = nullptr;
};
```

This avoids requiring a common base class or virtual destructor for all resource types. The deleter captures the concrete type at registration time and invokes `delete static_cast<T*>(ptr)` on destruction.

### Dual-Access Pattern

Resources are stored in parallel data structures, optimized for different access patterns:

| Structure | Purpose | Access Pattern |
|-----------|---------|----------------|
| `fastAccess_` | O(1) type-based lookup | Hot path: every system, every frame |
| `managers_` | Linear iteration over Managers | `init()`, `flushManagers()`, `reset()` |
| `commandBuffers_` | Linear iteration over CommandBuffers | `flush()` at commit points |
| `commandHandlers_` | CommandHandler lookup | TypedCommandBuffer flush routing |
| `owned_` | Lifetime management | Destruction only |

The same raw pointer appears in multiple vectors. This trades a few extra bytes of redundant pointers for zero-cost access in each use case.

### Two Registration Modes

#### 1. Owning Registration (Managers, CommandBuffers)

```cpp
auto& spawnMgr = registry.registerResource<SpawnManager>(args...);
```

The registry heap-allocates the resource, stores it in `owned_`, and indexes it in `fastAccess_`. If T derives from `Manager` or `CommandBuffer`, it is automatically added to the corresponding iteration list.

#### 2. Non-Owning Registration (CommandHandlers)

```cpp
registry.registerResource<TypedCommandHandler<SpawnCommand>>(spawnManager);
```

CommandHandlers are registered **by reference** — the registry does not own them. The handler typically lives as part of a Manager that is already owned. This is the mechanism Managers use to advertise which command types they handle: during `Manager::init()`, a Manager registers itself as a `TypedCommandHandler<T>` for each command type it processes.

## Integration with GameWorld

The ResourceRegistry is a member of `GameWorld` and accessed via `gameWorld.resourceRegistry()`:

```cpp
// Direct registry access
auto& poolMgr = gameWorld.resourceRegistry()
    .registerResource<GameObjectPoolManager>();

// Convenience methods on GameWorld delegate to the registry
auto& spawnMgr = gameWorld.registerManager<SpawnManager>();
T& mgr = gameWorld.manager<SpawnManager>();
```

### Initialization Sequence

1. **Registration:** Resources are registered via `registerResource()` or `registerManager()`.
2. **init():** `GameWorld::init()` iterates `managers_` and calls `init(gameWorld)` on each. Managers use `init()` to register their CommandHandlers.
3. **Runtime:** Systems access resources via `UpdateContext::resourceRegistry()`.
4. **Flush:** At each commit point, `GameWorld::flushManagers()` iterates `managers_` and calls `flush(updateContext)`.
5. **Reset:** `GameWorld::reset()` iterates `managers_` and calls `reset()` for level transitions.

```
Registration  →  init()  →  Runtime Loop  →  reset()
    │               │            │                │
    ▼               ▼            ▼                ▼
registerResource  mgr.init()  resource<T>()    mgr.reset()
                  (registers   tryResource<T>()
                   handlers)   flushManagers()
```

## Integration with Command System

The TypedCommandBuffer uses the ResourceRegistry to route commands to handlers at flush time:

```cpp
// During TypedCommandBuffer::flush():
if (registry.has<TypedCommandHandler<Cmd>>()) {
    auto& handler = registry.resource<TypedCommandHandler<Cmd>>();
    for (auto& cmd : queue) {
        handler.submit(cmd);
    }
} else if constexpr (ExecutableCommand<Cmd>) {
    for (auto& cmd : queue) {
        cmd.execute(updateContext);
    }
}
```

A Manager registers itself as handler during `init()`:

```cpp
void SpawnManager::init(GameWorld& gameWorld) noexcept override {
    gameWorld.registerCommandHandler<TypedCommandHandler<SpawnCommand>>(*this);
    gameWorld.registerCommandHandler<TypedCommandHandler<DespawnCommand>>(*this);
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

A custom Manager derives from `Manager`, implements `flush()`, and optionally overrides `init()` and `reset()`. If the Manager also handles commands, it additionally derives from `TypedCommandHandler<T>` for each command type.

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

import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.messaging.command.TypedCommandHandler;
import myapp.commands;

export namespace myapp {

    using namespace helios::engine::runtime::world;
    using namespace helios::engine::runtime::messaging::command;

    class CoinManager : public Manager,
                        public TypedCommandHandler<commands::CoinPickupCommand> {

        std::vector<commands::CoinPickupCommand> pending_;

    public:

        // Called by TypedCommandBuffer during flush
        bool submit(commands::CoinPickupCommand cmd) noexcept override {
            pending_.push_back(cmd);
            return true;
        }

        // Register this handler so the TypedCommandBuffer can route to it
        void init(GameWorld& gameWorld) override {
            gameWorld.registerCommandHandler<
                TypedCommandHandler<commands::CoinPickupCommand>
            >(*this);
        }

        // Process queued commands (called after CommandBuffer::flush)
        void flush(UpdateContext& ctx) noexcept override {
            for (const auto& cmd : pending_) {
                // award coins, update score, etc.
            }
            pending_.clear();
        }

        void reset() override {
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

At runtime, systems submit commands by looking up the handler via the ResourceRegistry. Note that `ctx.commandBuffer()` returns the `EngineCommandBuffer`, which only accepts engine-defined command types. Custom commands must be submitted directly to their handler:

```cpp
auto& handler = ctx.resourceRegistry()
    .resource<TypedCommandHandler<myapp::commands::CoinPickupCommand>>();
handler.submit(myapp::commands::CoinPickupCommand{collectorHandle, coinValue});
```

The Manager collects these submissions and processes them during `gameWorld.flushManagers()`.

Alternatively, if you have a [custom CommandBuffer](#custom-commandbuffers) that manages your application-specific commands, systems submit to that buffer instead, and it flushes alongside the engine buffers.

### Custom CommandBuffers

A custom CommandBuffer derives from `CommandBuffer` and implements `flush()` and `clear()`. This is useful for domain-specific command sets that should not pollute the engine-level `EngineCommandBuffer`.

```cpp
export module myapp.EditorCommandBuffer;

import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.world.UpdateContext;

export namespace myapp {

    class EditorCommandBuffer : public CommandBuffer {
        std::vector<UndoCommand> undoQueue_;
        std::vector<RedoCommand> redoQueue_;

    public:

        void addUndo(UndoCommand cmd) { undoQueue_.push_back(cmd); }
        void addRedo(RedoCommand cmd) { redoQueue_.push_back(cmd); }

        void flush(UpdateContext& ctx) noexcept override {
            for (auto& cmd : undoQueue_) { cmd.execute(ctx); }
            for (auto& cmd : redoQueue_) { cmd.execute(ctx); }
            clear();
        }

        void clear() noexcept override {
            undoQueue_.clear();
            redoQueue_.clear();
        }
    };
}
```

Register it alongside the EngineCommandBuffer:

```cpp
auto& editorBuf = gameWorld.resourceRegistry()
    .registerResource<myapp::EditorCommandBuffer>();
```

Because `EditorCommandBuffer` derives from `CommandBuffer`, it is automatically tracked in the `commandBuffers_` iteration list. Systems can look it up at runtime:

```cpp
auto* editorBuf = ctx.resourceRegistry()
    .tryResource<myapp::EditorCommandBuffer>();
if (editorBuf) {
    editorBuf->addUndo(UndoCommand{...});
}
```

### Registering Plain Resources

Not every resource needs to be a Manager or CommandBuffer. Any type can be registered for O(1) lookup:

```cpp
auto& config = gameWorld.resourceRegistry()
    .registerResource<GameConfig>(configPath);

// Later, in any system:
auto& cfg = ctx.resourceRegistry().resource<GameConfig>();
```

Plain resources are heap-allocated and owned by the registry via `ErasedUnique`, but they are not iterated during `init()`, `flush()`, or `reset()` — they are purely passive stores.

## Best Practices

- **Registration order matters:** Managers are initialized in registration order. Register dependencies first (e.g., `GameObjectPoolManager` before `SpawnManager`).
- **Use `tryResource()` for optional dependencies:** If a system can work without a resource, use `tryResource()` to check availability without asserting.
- **Register handlers in `init()`, not constructors:** The ResourceRegistry may not be fully populated during construction. `init()` is called after all resources are registered.
- **Single registration per type:** Each type can only be registered once. Attempting to register a duplicate triggers an assertion.

## Related Documentation

- [Command System](command-system.md) — TypedCommandBuffer flush routing via handlers
- [Game Loop Architecture](gameloop-architecture.md) — Commit points where managers are flushed
- [Object Pooling](object-pooling.md) — GameObjectPoolManager as a registered Manager
- [Spawn System](spawn-system.md) — SpawnManager as a registered Manager with CommandHandlers
- [State Management](state-management.md) — StateManagers as registered resources



