# Entity

An **Entity** is the primary interface for interacting with entities in the helios ECS. It provides a convenient, type-safe wrapper around an `EntityHandle` and the `EntityManager`, enabling component manipulation without direct access to low-level ECS internals.

> **Migration note:** `Entity` replaces the former `GameObject` class from `helios.engine.ecs`. It has been generalised with template parameters `THandle` and `TEntityManager` so it can work with any domain-specific handle and manager combination.

## Overview

`Entity` acts as a **facade** that combines:
- A `THandle` (typically `EntityHandle<TStrongId>`) identifying the entity
- A pointer to the `TEntityManager` that owns the entity's data

This design keeps the handle lightweight while providing rich functionality through the manager.

```cpp
// Entity is typically obtained from TypedHandleWorld
auto player = world.addEntity<GameHandle>();

// Add components
player.add<TransformComponent>(glm::vec3{0.0f});
player.add<HealthComponent>(100.0f);
player.add<RenderableComponent>(mesh);

// Query components
if (player.has<HealthComponent>()) {
    auto* health = player.get<HealthComponent>();
    health->takeDamage(10.0f);
}

// Activate/deactivate
player.setActive(false);  // Marks as inactive, calls onDeactivate()
player.setActive(true);   // Marks as active, calls onActivate()
```

## Template Parameters

```cpp
template<typename THandle, typename TEntityManager>
class Entity;
```

| Parameter | Description |
|-----------|-------------|
| `THandle` | The concrete `EntityHandle<TStrongId>` specialisation used as the entity identifier. |
| `TEntityManager` | The concrete `EntityManager<THandle, TEntityRegistry, TCapacity>` specialisation that owns component storage. |

### Type Aliases

Entity exposes the following member type aliases:

| Alias | Resolves To |
|-------|-------------|
| `ComponentTypeId_type` | `ComponentTypeId<THandle>` |
| `ComponentOpsRegistry_type` | `ComponentOpsRegistry<THandle>` |
| `HierarchyComponent_type` | `components::HierarchyComponent<THandle>` |

## API Reference

### Construction

Entities are created through `TypedHandleWorld`, not directly instantiated:

```cpp
// Correct — obtain from TypedHandleWorld
auto entity = world.addEntity<GameHandle>();

// Correct — lookup existing
auto entity = world.findEntity(handle);
```

Or constructed explicitly (advanced use):

```cpp
Entity<GameHandle, GameEM> entity{handle, &entityManager};
```

### Component Operations

| Method | Description |
|--------|-------------|
| `add<T>(args...)` | Constructs and attaches a component |
| `get<T>()` | Returns pointer to component (or nullptr) |
| `getOrAdd<T>(args...)` | Returns existing or creates new component |
| `has<T>()` | Checks if component is attached |
| `has(typeId)` | Checks by runtime type ID |
| `remove<T>()` | Removes a component, returns true if removed |
| `raw(typeId)` | Returns void* to component by type ID |

#### Adding Components

```cpp
// Add with constructor arguments
auto& transform = player.add<TransformComponent>(position, rotation, scale);

// Add with default constructor
auto& health = player.add<HealthComponent>();

// Get existing or add new
auto& collider = player.getOrAdd<CollisionComponent>();
```

#### Retrieving Components

```cpp
// Returns nullptr if not found
auto* transform = player.get<TransformComponent>();
if (transform) {
    transform->setPosition(newPos);
}

// Const version for read-only access
const auto* health = player.get<HealthComponent>();
```

#### Removing Components

```cpp
// Returns true if the component was removed
bool removed = player.remove<ShieldComponent>();
```

> **Note:** If the component implements `onRemove()` and returns `false`, the removal is cancelled and `remove<T>()` returns `false`.

### Activation State

Entities can be activated or deactivated, which propagates to all attached components:

```cpp
// Deactivate — adds Inactive tag, calls onDeactivate()
player.setActive(false);

// Activate — adds Active tag, calls onActivate()
player.setActive(true);

// Query state
if (player.isActive()) {
    // ...
}
```

When deactivated:
- An `Inactive` tag component is added
- The `Active` tag component is removed
- `onDeactivate()` is called on components that support it
- If a `HierarchyComponent` is present, it is marked dirty for propagation

When activated:
- The `Inactive` tag component is removed
- An `Active` tag component is added
- `onActivate()` is called on components that support it
- If a `HierarchyComponent` is present, it is marked dirty for propagation

> **Note:** `setActive()` does **not** call `enable()`/`disable()`. Use `enableComponent()`/`disableComponent()` for individual component toggling.

### Lifecycle Hooks

Entity provides methods to trigger lifecycle hooks on all attached components:

```cpp
// Called when acquired from object pool
player.onAcquire();

// Called when released back to pool
player.onRelease();
```

These methods iterate through all components and invoke the corresponding hooks if implemented.

### Component Iteration

```cpp
// Iterate over all component type IDs
for (auto typeId : player.componentTypeIds()) {
    void* raw = player.raw(typeId);
    // Process component...
}
```

### Entity Handle Access

```cpp
// Get the underlying handle
auto handle = player.handle();

// Handles can be stored and compared
if (handle == otherHandle) {
    // Same entity
}
```

## Internal Structure

```cpp
template<typename THandle, typename TEntityManager>
class Entity {
    THandle entityHandle_;              // EntityHandle<TStrongId>
    TEntityManager* entityManager_;     // Pointer to owning manager
    
    // ...methods delegate to entityManager_
};
```

The `Entity` itself is:
- **Lightweight** (~16 bytes) — smaller than most smart pointers
- **Copyable** — multiple Entity instances can reference the same entity
- **Non-owning** — does not manage entity lifetime
- **Pass-by-value friendly** — should be passed by value, not by reference

### Pass by Value

Due to its small size, `Entity` should be passed **by value** rather than by reference or pointer:

```cpp
// Correct — pass by value
void processEntity(Entity<GameHandle, GameEM> entity) {
    entity.get<HealthComponent>()->takeDamage(10.0f);
}

// Unnecessary — reference adds indirection for no benefit
void processEntity(Entity<GameHandle, GameEM>& entity);       // Avoid
void processEntity(const Entity<GameHandle, GameEM>& entity);  // Avoid
```

Copying an `Entity` is essentially copying a handle and a pointer — faster than dereferencing a reference in most cases.

## Relationship with Other Classes

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
│    └──────────────┘                                         │
└─────────────────────────────────────────────────────────────┘
```

## Best Practices

1. **Don't store Entity long-term** — Store `EntityHandle` instead and resolve via `TypedHandleWorld::findEntity()`

2. **Check component existence** — Use `has<T>()` before `get<T>()` or handle nullptr returns

3. **Use getOrAdd for optional components** — Simplifies code when components may or may not exist

4. **Prefer type-safe methods** — Use `has<T>()` over `has(typeId)` when possible

5. **Leverage setActive for pooling** — Deactivate instead of destroying for pooled objects

## Example: Complete Entity Setup

```cpp
auto enemy = world.addEntity<GameHandle>();

// Transform
enemy.add<ComposeTransformComponent>();
enemy.add<TranslationStateComponent>(spawnPosition);
enemy.add<ScaleStateComponent>(enemySize);
enemy.add<RotationStateComponent>();

// Rendering
auto& renderable = enemy.add<RenderableComponent>(mesh);
enemy.add<SceneNodeComponent>(sceneNode);
enemy.add<ModelAabbComponent>(renderable.localAABB());

// Physics
enemy.add<CollisionComponent>(CollisionLayer::Enemy);
enemy.add<AabbColliderComponent>();
enemy.add<Move2DComponent>(speed);
enemy.add<DirectionComponent>();

// Gameplay
enemy.add<HealthComponent>(100.0f);
enemy.add<DamageDealerComponent>();

// Pooling support
enemy.add<SpawnedByProfileComponent>();
```

## See Also

- [EntityManager](entity-manager.md) - Low-level entity and component management
- [EntityHandle](entity-handle.md) - Versioned entity reference
- [TypedHandleWorld](typed-handle-world.md) - Multi-domain world
- [View](view.md) - Component-based entity queries
- [System](system.md) - Game logic processors
- [Component Lifecycle](../component-lifecycle.md) - Lifecycle hooks
- [Component System](../component-system.md) - ECS architecture overview
- [Sparse Set](../sparse-set.md) - Underlying component storage

