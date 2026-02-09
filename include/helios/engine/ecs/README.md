# helios::engine::ecs

Core Entity-Component-System architecture for the helios game engine.

## Overview

This module provides the foundational classes for the composition-based game architecture. It separates data (Components) from behavior (Systems). Entity lifecycle and world management are provided by `helios.engine.runtime.world`.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObject` | Lightweight entity facade (~16 bytes, pass-by-value) |
| `EntityHandle` | Versioned handle for safe entity references (8 bytes) |
| `EntityRegistry` | Handle allocation and validation |
| `EntityManager` | Unified interface for entity creation and component storage |
| `SparseSet<T>` | Generic O(1) data structure for component storage |
| `SparseSetBase` | Type-erased base for polymorphic sparse set access |
| `View` | Component-based entity queries |
| `System` | Abstract base for logic processors |
| `Updatable` | Interface for per-frame updatable objects |
| `ComponentOps` | Function pointers for lifecycle callbacks |
| `ComponentOpsRegistry` | Global registry mapping type IDs to ComponentOps |
| `ComponentReflector` | Compile-time type registration |
| `Traits` | Compile-time concepts for component lifecycle hooks |

## Component Storage Model

`GameObject` uses a **type-indexed storage** system for components. Instead of a hash-based lookup, components are stored in a contiguous vector where the index corresponds to the component's compile-time `ComponentTypeId`.

**Performance Characteristics:**

| Operation | Complexity | Notes |
|-----------|------------|-------|
| `get<T>()` | O(1) | Direct array access via type ID |
| `has<T>()` | O(1) | Bounds check + nullptr check |
| `add<T>()` | O(1) amortized | May trigger resize if type ID exceeds capacity |
| `components()` | O(1) / O(n) | O(1) if cache valid, O(n) on first access after modification |

**Trade-offs:**
- **Memory**: Sparse vector with nullptr slots for unused component types.
- **Speed**: Direct indexing without hash computation or collision handling.
- **Cache**: Filtered component views are cached and lazily rebuilt.

```cpp
// Storage layout example (conceptual):
// Index:      [0]       [1]       [2]       [3]       ...
// Content:  nullptr  Transform  nullptr   Move2D    ...
//                       ↑                    ↑
//           ComponentTypeId::id<Transform>()  ComponentTypeId::id<Move2D>()
```

## EntityRegistry and EntityManager

The **EntityRegistry** is the single source of truth for entity lifecycle. It manages handle allocation, version tracking, and entity validation.

The **EntityManager** provides a high-level API combining registry and component storage:

```cpp
EntityRegistry registry;
EntityManager manager(registry);

// Create entity
auto entity = manager.create();

// Attach components
auto* transform = manager.emplace<TransformComponent>(entity, glm::vec3{0.0f});
auto* velocity = manager.emplace<VelocityComponent>(entity);

// Check and retrieve component
if (manager.has<TransformComponent>(entity)) {
    auto* t = manager.get<TransformComponent>(entity);
}

// Remove single component
manager.remove<TransformComponent>(entity);

// Destroy entity (removes all components and invalidates handle)
manager.destroy(entity);
```

**Architecture:**

```
┌────────────────────────────────────────────────────────────┐
│                     EntityManager                          │
│  ┌──────────────────┐    ┌───────────────────────────────┐ │
│  │  EntityRegistry  │    │  Component Storage            │ │
│  │  (handle mgmt)   │    │  vector<SparseSet<T>>         │ │
│  │  - create()      │    │  (indexed by TypeIndexer)     │ │
│  │  - destroy()     │    │                               │ │
│  │  - isValid()     │    │  [0] SparseSet<Transform>     │ │
│  └──────────────────┘    │  [1] SparseSet<Velocity>      │ │
│                          │  [2] SparseSet<Health>        │ │
│                          └───────────────────────────────┘ │
└────────────────────────────────────────────────────────────┘
```

## EntityPool

`EntityPool<T>` is a sparse-set based data structure for storing entities with versioned handles. It provides O(1) insertion, lookup, and cache-friendly iteration.

**Data Structure:**

```
sparse_:  [0]  [1]  [2]  [3]  ...     (maps entity ID → dense index)
           ↓    ↓    ↓    ↓
dense_:   [0]  [2]  [3]  ...          (entity IDs, contiguous)
denseData_: [E0] [E2] [E3] ...        (actual entities, cache-friendly)
version_: [1]  [1]  [2]  [1]  ...     (version per ID, incremented on removal)
```


**Versioned Handles:**

`EntityHandle` contains both an entity ID and a version number. When an entity is removed, its version is incremented. This allows detection of stale handles that reference deleted entities.

```cpp
// EntityPool usage example
EntityPool<GameObject> pool;

// Add entity, receive handle
auto handle = pool.emplace(std::make_unique<GameObject>());

// Retrieve entity via handle (O(1))
auto* entity = pool.get(handle);

// Iterate all entities (cache-friendly)
for (auto& e : pool.entities()) {
    // process entity
}
```

## Submodules

### Traits

Compile-time concepts for component lifecycle hooks:

| Trait | Purpose |
|-------|---------|
| `HasOnRemove` | Intercept removal (return `false` to cancel) |
| `HasOnAcquire` | Called when acquired from a pool |
| `HasOnRelease` | Called when released back to a pool |

```cpp
struct ResourceComponent {
    bool onRemove() {
        cleanup();
        return true;  // Allow removal
    }
};

static_assert(helios::engine::ecs::traits::HasOnRemove<ResourceComponent>);
```

### query/

Entity query utilities for filtering and iterating GameObjects:

| Class | Purpose |
|-------|---------|
| `GameObjectFilter` | Bitmask for filtering by active/enabled state |
| `GameObjectView` | Lazy range for component-based iteration |

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.runtime.world` | GameWorld, Level, UpdateContext, Manager, SystemRegistry |
| `helios.engine.runtime.gameloop` | GameLoop, Phase, Pass |

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        GameWorld                            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │ GameObjects │  │  Managers   │  │  Pools/Registry     │  │
│  │  (entities) │  │ (deferred)  │  │  (object recycling) │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
          │                │
          ▼                ▼
┌─────────────────────────────────────────────────────────────┐
│                        GameLoop                             │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐                      │
│  │   Pre   │─>│  Main   │─>│  Post   │  (Phases)            │
│  │ Phase   │  │ Phase   │  │ Phase   │                      │
│  └─────────┘  └─────────┘  └─────────┘                      │
│       │            │            │                           │
│       ▼            ▼            ▼                           │
│    Systems      Systems      Systems     (execute in order) │
└─────────────────────────────────────────────────────────────┘
```

## Usage Example

```cpp
import helios.engine.ecs;
import helios.engine.runtime.world.GameWorld;

// Create game world
helios::engine::runtime::world::GameWorld world;

// Add entity with components
auto entity = std::make_unique<helios::engine::ecs::GameObject>();
entity.add<Move2DComponent>();
entity.add<SceneNodeComponent>(sceneNode);
auto* player = world.addGameObject(std::move(entity));

// Query entities by component
for (auto [obj, move] : world.find<Move2DComponent>()) {
    // Process matching entities
}
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::ecs
@brief Core Entity-Component-System architecture.
@details This module provides the foundational classes for the composition-based game architecture. It includes entity base classes (GameObject, Component, CloneableComponent), system infrastructure (System), and query utilities (GameObjectFilter, GameObjectView).
</p></details>

