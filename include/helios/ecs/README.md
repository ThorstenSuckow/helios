# helios::ecs

Generic, reusable ECS primitives shared across the engine.

## Overview

This module provides the complete set of building blocks for entity management,
component storage, querying and lifecycle reflection. All types are
**policy-based templates** that can be specialised for different domains
(e.g. game entities, UI widgets, audio sources).

The classes were previously located in `helios.ecs` (which is now
retained only for backward compatibility) and have been generalised through
template parameters so they are independent of any specific engine subsystem.

## Module Structure

The `helios.ecs` module exports the following types:

### Entity Management

| Class | Template Parameters | Purpose |
|-------|---------------------|---------|
| `EntityHandle<TStrongId>` | `TStrongId` — domain-specific strong ID type | Versioned, strongly-typed entity reference |
| `EntityRegistry<TStrongId, TLookupStrategy, TAllowRemoval, TCapacity>` | Strong ID type, lookup strategy, removal policy, capacity | Handle allocation, version tracking, validation |
| `EntityManager<THandle, TEntityRegistry, TCapacity>` | Handle type, registry type, capacity | Unified entity creation and component storage |
| `Entity<THandle, TEntityManager>` | Handle type, manager type | Lightweight entity facade (~16 bytes, pass-by-value) |
| `EntityResolver<TEntityManager>` | Manager type | Callable for resolving handles to Entity wrappers |
| `TypedHandleWorld<TEntityManagers...>` | One or more EntityManager specialisations | Multi-domain world with compile-time handle dispatch |

### Component Infrastructure

| Class | Template Parameters | Purpose |
|-------|---------------------|---------|
| `ComponentTypeId<THandle>` | Handle type (scoping) | Compile-time unique type identifier for components |
| `ComponentOps` | *(none)* | Type-erased function pointers for lifecycle callbacks |
| `ComponentOpsRegistry<THandle>` | Handle type (scoping) | Global O(1) registry mapping type IDs to ComponentOps |
| `ComponentReflector<THandle, TEntityManager>` | Handle type, manager type | Compile-time trait-based lifecycle registration |

### Storage & Iteration

| Class | Template Parameters | Purpose |
|-------|---------------------|---------|
| `SparseSet<T>` | Element type | Generic O(1) entity-keyed storage |
| `SparseSetBase` | *(none)* | Type-erased base for polymorphic sparse set access |
| `View<TEntityManager, Components...>` | Manager type, component types | Lightweight component-based entity queries |

### Lookup Strategies

| Strategy | Lookup | Insertion | Removal | Use Case |
|----------|--------|-----------|---------|----------|
| `HashedLookupStrategy` | O(1) avg | O(1) avg | O(1) avg | Default — large registries |
| `LinearLookupStrategy` | O(n) | O(n) | O(1) | Small registries |

### Traits (Compile-Time Concepts)

| Trait | Required Method(s) | Purpose |
|-------|--------------------|---------|
| `HasOnAcquire` | `onAcquire()` | Pool acquisition callback |
| `HasOnRelease` | `onRelease()` | Pool release callback |
| `HasOnRemove` | `onRemove() → bool` | Removal interception |
| `HasToggleable` | `enable()`, `disable()` | Component enable/disable |
| `HasClone` | `onClone(const T&)` | Post-copy initialisation |
| `HasActivatable` | `onActivate()`, `onDeactivate()` | Entity activation response |

## Template Parameter Overview

The move from `helios.engine.ecs` to `helios.ecs` introduced the
following **new** template parameters:

| Parameter | Used In | Purpose |
|-----------|---------|---------|
| `TStrongId` | `EntityHandle`, `EntityRegistry` | Domain-specific strong ID type satisfying `IsStrongIdLike` |
| `THandle` | `EntityManager`, `Entity`, `ComponentTypeId`, `ComponentOpsRegistry`, `ComponentReflector` | The concrete `EntityHandle<TStrongId>` specialisation, scoping type IDs and ops per domain |
| `TEntityRegistry` | `EntityManager` | The concrete `EntityRegistry` specialisation used for handle allocation |
| `TEntityManager` | `Entity`, `View`, `EntityResolver`, `ComponentReflector` | The concrete `EntityManager` specialisation providing component storage |
| `TLookupStrategy` | `EntityRegistry` | Pluggable collision detection strategy for strong IDs |
| `TAllowRemoval` | `EntityRegistry` | `bool` policy — `false` disables `destroy()` at compile time |
| `TCapacity` | `EntityRegistry`, `EntityManager` | Default initial capacity hint |

## TypedHandleWorld

`TypedHandleWorld<TEntityManagers...>` is a **multi-domain entity world**
that holds one `EntityManager` per domain inside a `std::tuple`. The handle
type of each manager serves as a compile-time key: calling
`world.addEntity<MyHandle>()` selects the correct manager automatically.

```cpp
using GameHandle = EntityHandle<GameStrongId>;
using UiHandle   = EntityHandle<UiStrongId>;

using GameEM = EntityManager<GameHandle, GameRegistry, 4096>;
using UiEM   = EntityManager<UiHandle,   UiRegistry,   512>;

TypedHandleWorld<GameEM, UiEM> world;

auto player = world.addEntity<GameHandle>();   // → GameEM
auto button = world.addEntity<UiHandle>();     // → UiEM
```

## Quick Start

```cpp
import helios.ecs;

// 1. Define domain types
struct MyStrongId { /* ... */ };
using Handle   = helios::ecs::types::EntityHandle<MyStrongId>;
using Registry = helios::ecs::EntityRegistry<MyStrongId>;
using Manager  = helios::ecs::EntityManager<Handle, Registry, 1024>;

// 2. Create manager and entity
Manager em;
auto handle = em.create();

// 3. Attach components
auto* transform = em.emplace<TransformComponent>(handle, position);

// 4. Use the Entity facade
helios::ecs::Entity<Handle, Manager> entity{handle, &em};
entity.add<HealthComponent>(100.0f);

// 5. Iterate via View
for (auto [e, t, v] :
     helios::ecs::View<Manager, TransformComponent, VelocityComponent>(&em)) {
    // process
}
```

## See Also

- [Engine ECS](../../engine/ecs/README.md) — backward-compatible wrappers (`GameObject`, etc.)
- [Core Types](../types.ixx) — `EntityId`, `VersionId`, `EntityTombstone`, `StrongId_t`

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs
@brief Generic, reusable ECS primitives.
@details Provides policy-based templates for entity handle management, version
tracking, sparse-set storage, component lifecycle reflection, typed views and
multi-domain entity worlds that are independent of any specific engine subsystem.
</p></details>
