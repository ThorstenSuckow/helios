# helios::ecs

Generic, reusable ECS primitives shared across the engine.

## Overview

`helios::ecs` provides policy-based templates for entity identity, storage,
lookup, lifecycle hooks, and typed views. The module is domain-agnostic and
can be specialized for game entities, UI entities, render resources, or
platform/runtime entities.

## Module Structure

### Entity Management

| Type | Purpose |
|------|---------|
| `EntityHandle<TStrongId>` | Versioned, strongly-typed entity reference |
| `EntityRegistry<TStrongId, TLookupStrategy, TAllowRemoval, TCapacity>` | Handle allocation, version tracking, validation |
| `EntityManager<THandle, TEntityRegistry, TCapacity>` | Entity creation, destruction, component storage |
| `Entity<TManager>` | Lightweight entity facade (handle + manager pointer) |
| `EntityResolver<TEntityManager>` | Callable handle-to-entity resolver |
| `TypedHandleWorld<TEntityManagers...>` | Multi-domain world with compile-time handle dispatch |

### Component Infrastructure

| Type | Purpose |
|------|---------|
| `ComponentTypeId<THandle>` | Domain-scoped component type ids |
| `ComponentOps` | Type-erased lifecycle callback function set |
| `ComponentOpsRegistry<THandle>` | Runtime map: type id -> component ops |
| `ComponentReflector<THandle, TEntityManager>` | Trait-based registration of lifecycle hooks |

### Storage & Query

| Type | Purpose |
|------|---------|
| `SparseSet<T>` | Generic O(1)-style sparse-set component storage |
| `View<TEntityManager, Components...>` | Typed component query/iteration |

### Lookup Strategies

| Strategy | Purpose |
|----------|---------|
| `HashedLookupStrategy` | Hash-based strong-id collision tracking |
| `LinearLookupStrategy<TCapacity>` | Flat-vector collision tracking for small sets |

### Concepts & Traits

| Type | Purpose |
|------|---------|
| `IsEntityHandle` | Constraint for `EntityHandle<TStrongId>` shapes |
| `IsStrongIdCollisionResolverLike` | Constraint for registry lookup strategies |
| `traits::HasOnAcquire`, `traits::HasOnRelease`, `traits::HasOnRemove` | Optional pool/remove hooks |
| `traits::HasToggleable`, `traits::HasClone`, `traits::HasActivatable` | Optional component lifecycle hooks |

## TypedHandleWorld

`TypedHandleWorld<TEntityManagers...>` stores multiple manager types and routes
calls by handle type at compile time:

```cpp
using GameHandle = EntityHandle<GameStrongId>;
using UiHandle   = EntityHandle<UiStrongId>;

using GameEM = EntityManager<GameHandle, GameRegistry, 4096>;
using UiEM   = EntityManager<UiHandle, UiRegistry, 512>;

TypedHandleWorld<GameEM, UiEM> world;

auto player = world.addEntity<GameHandle>();
auto button = world.addEntity<UiHandle>();
```

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `components/` | Core ECS marker/relationship components |
| `concepts/` | Concepts and lifecycle trait constraints |
| `strategies/` | Strong-id collision lookup strategies |
| `types/` | Handle ids, component ids, and ECS typedefs |

## See Also

- [Components](components/README.md) — ECS core components
- [Concepts](concepts/README.md) — ECS-specific constraints and lifecycle traits
- [Core Concepts](../core/concepts/README.md) — shared constraints such as `IsStrongIdLike`
- [Strategies](strategies/README.md) — lookup strategy implementations
- [Types](types/README.md) — ECS type layer
- [Core Concepts: ECS](../../../docs/core-concepts/ecs/README.md) — architecture-oriented ECS documentation

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs
@brief Generic, reusable ECS primitives.
@details Provides policy-based templates for handle management, sparse-set storage,
component lifecycle reflection, typed queries, and multi-domain ECS worlds.
</p></details>
