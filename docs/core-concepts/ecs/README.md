# ECS Architecture

The **Entity-Component-System (ECS)** architecture in helios separates data (Components) from behavior (Systems), enabling flexible and cache-friendly game object composition.

## Overview

```
┌─────────────────────────────────────────────────────────────┐
│                   TypedHandleWorld                           │
│  ┌──────────────────┐    ┌────────────────────────────┐     │
│  │  EntityRegistry  │◄───│      EntityManager         │     │
│  │  (handle alloc)  │    │  (component storage)       │     │
│  └──────────────────┘    └────────────────────────────┘     │
│           │                          ▲                      │
│           ▼                          │                      │
│    ┌──────────────┐           ┌──────────────┐              │
│    │ EntityHandle │◄──────────│    Entity    │              │
│    │ (id+version  │           │   (facade)   │              │
│    │  +strongId)  │           └──────────────┘              │
│    └──────────────┘                  │                      │
│                               ┌──────┴──────┐               │
│                               │    View     │               │
│                               │  (queries)  │               │
│                               └─────────────┘               │
└─────────────────────────────────────────────────────────────┘
                    │
        ┌───────────┴───────────┐
        ▼                       ▼
┌───────────────────┐   ┌───────────────────────────────────┐
│     Systems       │   │     Component Reflection          │
│  ┌──────────────┐ │   │  ┌─────────────┐ ┌─────────────┐  │
│  │   System     │ │   │  │ComponentOps │ │  Traits     │  │
│  │ (type-erased)│ │   │  │  Registry   │ │ (concepts)  │  │
│  └──────────────┘ │   │  └─────────────┘ └─────────────┘  │
└───────────────────┘   └───────────────────────────────────┘
```

## Module Structure

The `helios.core.ecs` module exports the following classes. All types are
**policy-based templates**, parameterised on handle, registry and manager types
so they can be specialised for different domains without code duplication.

| Class | Purpose | 
|-------|---------|
| [Component Structure](component-structure.md) | Required structure for components (Copy/Move) |
| [Entity](entity.md) | High-level entity facade (~16 bytes, pass-by-value) | 
| [EntityHandle](entity-handle.md) | Versioned, strongly-typed entity reference | 
| [EntityRegistry](entity-registry.md) | Handle allocation & validation | 
| [EntityManager](entity-manager.md) | Component storage via SparseSets | 
| [TypedHandleWorld](typed-handle-world.md) | Multi-domain world with compile-time handle dispatch |
| [View](view.md) | Component-based entity queries | 
| [System](system.md) | Type-erased wrapper for game logic processors |
| [SystemRegistry](system.md#systemregistry) | Type-indexed registry for system instances |
| [SparseSet](../sparse-set.md) | O(1) component storage | 
| [Traits](traits.md) | Compile-time lifecycle hook detection | 
| [ComponentOps](component-ops.md) | Function pointers for lifecycle callbacks | 
| [ComponentOpsRegistry](component-ops.md#componentopsregistry) | Global registry for ComponentOps | 
| [ComponentReflector](../component-registry.md) | Type registration helper | 
| [EntityResolver](entity-resolver.md) | Callable for resolving handles to Entity wrappers |

## Template Parameterisation

The core ECS types are templated so that **each domain** (game entities,
UI elements, audio sources, …) can have its own handle type, registry and
manager — with all type relationships resolved at compile time.

| Parameter | Used In | Purpose |
|-----------|---------|---------|
| `TStrongId` | `EntityHandle`, `EntityRegistry` | Domain-specific strong ID type |
| `THandle` | `EntityManager`, `Entity`, `ComponentTypeId`, `ComponentOpsRegistry`, `ComponentReflector` | Concrete `EntityHandle<TStrongId>` — scopes type IDs per domain |
| `TEntityRegistry` | `EntityManager` | Concrete `EntityRegistry` specialisation |
| `TEntityManager` | `Entity`, `View`, `EntityResolver`, `ComponentReflector` | Concrete `EntityManager` specialisation |
| `TLookupStrategy` | `EntityRegistry` | Pluggable strong ID collision detection |
| `TAllowRemoval` | `EntityRegistry` | `false` disables `destroy()` at compile time |
| `TCapacity` | `EntityRegistry`, `EntityManager` | Default initial capacity hint |

## Quick Start

```cpp
import helios.core.ecs;

// 1. Define types for your domain
using GameHandle  = EntityHandle<GameStrongId>;
using GameReg     = EntityRegistry<GameStrongId>;
using GameEM      = EntityManager<GameHandle, GameReg, 4096>;

// 2. Use TypedHandleWorld for multi-domain setups
TypedHandleWorld<GameEM> world;

auto player = world.addEntity<GameHandle>();

// 3. Add components
player.add<TransformComponent>(position);
player.add<HealthComponent>(100.0f);
player.add<VelocityComponent>();

// 4. Query entities in a system
void update(UpdateContext& ctx) noexcept {
    for (auto [entity, transform, velocity, active] : ctx.view<
        TransformComponent,
        VelocityComponent,
        Active
    >().whereEnabled()) {
        transform->position += velocity->direction * ctx.deltaTime();
    }
}
```

## Data Flow

1. **Entity Creation**: `EntityRegistry` allocates versioned handle with strong ID
2. **Component Attachment**: `EntityManager` stores in type-indexed `SparseSet`
3. **Queries**: `View` iterates entities with matching components
4. **Updates**: `System` processes entities each frame
5. **Destruction**: Handle version incremented, index recycled

## Key Features

### Versioned Handles with Strong IDs
Detect stale references and carry domain semantics:
```cpp
auto handle = registry.create();       // {entityId, version, strongId}
registry.destroy(handle);
registry.isValid(handle);              // false — version mismatch
```

### Domain-Specific Entity Managers
`TypedHandleWorld` dispatches to the correct manager at compile time:
```cpp
TypedHandleWorld<GameEM, UiEM> world;
auto player = world.addEntity<GameHandle>();  // → GameEM
auto button = world.addEntity<UiHandle>();    // → UiEM
```

### Type-Indexed Storage
O(1) component access without hash lookups:
```cpp
auto* health = entity.get<HealthComponent>();  // Direct array access
```

### Cache-Friendly Iteration
Contiguous memory layout in `SparseSet`:
```cpp
for (auto [e, t, v, a] : view<Transform, Velocity, Active>()) {
    // Components stored contiguously
}
```

### Compile-Time Lifecycle Detection
Traits detect hooks without runtime overhead:
```cpp
if constexpr (traits::HasOnAcquire<T>) {
    component->onAcquire();
}
```

## Documentation

### Entity Management
- [Entity](entity.md) - The entity facade you'll use most
- [EntityHandle](entity-handle.md) - How entity references work
- [EntityRegistry](entity-registry.md) - Handle lifecycle internals
- [EntityManager](entity-manager.md) - Component storage details
- [TypedHandleWorld](typed-handle-world.md) - Multi-domain world

### Querying & Processing
- [View](view.md) - Efficient component queries
- [System](system.md) - Writing game logic

### Storage & Reflection
- [SparseSet](../sparse-set.md) - The underlying data structure
- [Traits](traits.md) - Compile-time hook detection
- [ComponentOps](component-ops.md) - Lifecycle function pointers
- [Component Registry](../component-registry.md) - Type registration
- [Component Lifecycle](../component-lifecycle.md) - Hooks and callbacks

## See Also

- [Component System](../component-system.md) - High-level ECS concepts
- [Game Loop Architecture](../gameloop-architecture.md) - How systems execute
- [Spawn System](../spawn-system.md) - Entity pooling and spawning

