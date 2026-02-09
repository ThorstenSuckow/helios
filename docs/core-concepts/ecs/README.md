# ECS Architecture

The **Entity-Component-System (ECS)** architecture in helios separates data (Components) from behavior (Systems), enabling flexible and cache-friendly game object composition.

## Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        GameWorld                            │
│  ┌──────────────────┐    ┌────────────────────────────┐     │
│  │  EntityRegistry  │◄───│      EntityManager         │     │
│  │  (handle alloc)  │    │  (component storage)       │     │
│  └──────────────────┘    └────────────────────────────┘     │
│           │                          ▲                      │
│           ▼                          │                      │
│    ┌──────────────┐           ┌──────────────┐              │
│    │ EntityHandle │◄──────────│  GameObject  │              │
│    │ (id+version) │           │   (facade)   │              │
│    └──────────────┘           └──────────────┘              │
│                                      │                      │
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
│  │  (Updatable) │ │   │  │  Registry   │ │ (concepts)  │  │
│  └──────────────┘ │   │  └─────────────┘ └─────────────┘  │
└───────────────────┘   └───────────────────────────────────┘
```

## Module Structure

The `helios.engine.ecs` module exports the following classes:

| Class | Purpose | 
|-------|---------|
| [Component Structure](component-structure.md) | Required structure for components (Copy/Move) |
| [GameObject](gameobject.md) | High-level entity facade (~16 bytes, pass-by-value) | 
| [EntityHandle](entity-handle.md) | Versioned entity reference (8 bytes) | 
| [EntityRegistry](entity-registry.md) | Handle allocation & validation | 
| [EntityManager](entity-manager.md) | Component storage via SparseSets | 
| [View](view.md) | Component-based entity queries | 
| [System](system.md) | Game logic processor base class | 
| [Updatable](updatable.md) | Interface for per-frame updates | 
| [SparseSet](../sparse-set.md) | O(1) component storage | 
| [Traits](traits.md) | Compile-time lifecycle hook detection | 
| [ComponentOps](component-ops.md) | Function pointers for lifecycle callbacks | 
| [ComponentOpsRegistry](component-ops.md#componentopsregistry) | Global registry for ComponentOps | 
| [ComponentReflector](../component-registry.md) | Type registration helper | 

## Quick Start

```cpp
// 1. Get a GameObject from the world
auto player = gameWorld.addGameObject();

// 2. Add components
player.add<TransformComponent>(position);
player.add<HealthComponent>(100.0f);
player.add<VelocityComponent>();

// 3. Query entities in systems
for (auto [entity, transform, velocity, active] : gameWorld.view<
    TransformComponent,
    VelocityComponent,
    Active
>().whereEnabled()) {
    transform->position += velocity->direction * deltaTime;
}
```

## Data Flow

1. **Entity Creation**: `EntityRegistry` allocates versioned handle
2. **Component Attachment**: `EntityManager` stores in type-indexed `SparseSet`
3. **Queries**: `View` iterates entities with matching components
4. **Updates**: `System` processes entities each frame
5. **Destruction**: Handle version incremented, index recycled

## Key Features

### Versioned Handles
Detect stale references to destroyed entities:
```cpp
auto handle = registry.create();
registry.destroy(handle);
registry.isValid(handle);  // false
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
// Detected at compile time via concepts
if constexpr (traits::HasOnAcquire<T>) {
    component->onAcquire();
}
```

## Documentation

### Entity Management
- [GameObject](gameobject.md) - The entity facade you'll use most
- [EntityHandle](entity-handle.md) - How entity references work
- [EntityRegistry](entity-registry.md) - Handle lifecycle internals
- [EntityManager](entity-manager.md) - Component storage details

### Querying & Processing
- [View](view.md) - Efficient component queries
- [System](system.md) - Writing game logic
- [Updatable](updatable.md) - Per-frame update interface

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


