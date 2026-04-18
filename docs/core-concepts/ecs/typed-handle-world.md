# TypedHandleWorld

A **TypedHandleWorld** is a multi-domain entity world that holds multiple `EntityManager` instances and dispatches operations to the correct one based on the entity handle type — all resolved at **compile time**.

## Overview

`TypedHandleWorld` enables a single world object to manage entities from
multiple independent registries. For example, game entities, UI entities
and audio entities can coexist in one world without runtime dispatch overhead.

```cpp
// Define domain-specific types
using GameHandle = EntityHandle<GameStrongId>;
using UiHandle   = EntityHandle<UiStrongId>;

using GameEM = EntityManager<GameHandle, GameRegistry, 4096>;
using UiEM   = EntityManager<UiHandle,   UiRegistry,   512>;

// Create the world
TypedHandleWorld<GameEM, UiEM> world;

// The handle type selects the correct EntityManager automatically
auto player = world.addEntity<GameHandle>();   // → GameEM
auto button = world.addEntity<UiHandle>();     // → UiEM

// Queries are also domain-scoped
for (auto [entity, transform, velocity] :
     world.view<GameHandle, TransformComponent, VelocityComponent>()) {
    // Only iterates game entities
}
```

## Template Parameters

```cpp
template<typename... TEntityManagers>
class TypedHandleWorld;
```

| Parameter | Description |
|-----------|-------------|
| `TEntityManagers...` | One or more `EntityManager` specialisations. Each must expose a **unique** `Handle_type` typedef. |

## Architecture

### Compile-Time Handle Dispatch

`TypedHandleWorld` stores all managers in a `std::tuple<TEntityManagers...>`.
When a method is called with a handle-type template argument, the helper trait
`HandleToManager` recursively walks the parameter pack at compile time and
returns the zero-based index of the matching manager.

```
TypedHandleWorld<GameEM, UiEM>
│
├── std::tuple
│   ├── [0] GameEM   (Handle_type = GameHandle)
│   └── [1] UiEM     (Handle_type = UiHandle)
│
│  addEntity<GameHandle>()   →  HandleToManager finds index 0  →  GameEM
│  addEntity<UiHandle>()     →  HandleToManager finds index 1  →  UiEM
```

A `static_assert` fires at compile time if no manager matches the requested
handle type.

### Relationship Diagram

```
┌───────────────────────────────────────────────────────────────┐
│                    TypedHandleWorld                           │
│                                                               │
│  ┌─────────────────────┐     ┌─────────────────────┐          │
│  │     GameEM          │     │       UiEM          │          │
│  │  Handle_type =      │     │  Handle_type =      │          │
│  │    GameHandle       │     │    UiHandle         │          │
│  │  ┌───────────────┐  │     │  ┌───────────────┐  │          │
│  │  │EntityRegistry │  │     │  │EntityRegistry │  │          │
│  │  └───────────────┘  │     │  └───────────────┘  │          │
│  │  ┌───────────────┐  │     │  ┌───────────────┐  │          │
│  │  │ SparseSet<T>  │  │     │  │ SparseSet<T>  │  │          │
│  │  │ SparseSet<U>  │  │     │  │ SparseSet<V>  │  │          │
│  │  └───────────────┘  │     │  └───────────────┘  │          │
│  └─────────────────────┘     └─────────────────────┘          │
└───────────────────────────────────────────────────────────────┘
```

## API Reference

### entityManager\<THandle\>()

Returns a reference to the `EntityManager` whose `Handle_type` matches
`THandle`:

```cpp
auto& gameEM = world.entityManager<GameHandle>();
auto& uiEM   = world.entityManager<UiHandle>();

// Also available as const
const auto& gameEM = world.entityManager<GameHandle>();
```

### addEntity\<THandle\>()

Creates a new entity in the domain identified by `THandle` and returns an
`Entity` facade:

```cpp
auto player = world.addEntity<GameHandle>();

player.add<TransformComponent>(position);
player.add<HealthComponent>(100.0f);
```

### addEntity\<THandle\>(strongId)

Creates a new entity in the domain identified by `THandle` using an explicit
strong id value:

```cpp
auto shader = world.addEntity<ShaderHandle>(ShaderId{"player_shader"});
```

### destroy\<THandle\>(handle)

Destroys an entity in the domain identified by `THandle`:

```cpp
const bool removed = world.destroy<GameHandle>(player.handle());
```

Returns `true` if the handle was valid and the entity was destroyed.

### findEntity\<THandle\>(handle)

Looks up an existing entity by its handle. Returns `std::optional<Entity>` —
engaged if the handle is valid:

```cpp
auto result = world.findEntity(someHandle);
if (result) {
    auto entity = *result;
    entity.get<HealthComponent>()->heal(50.0f);
}
```

### view\<THandle, Components...\>()

Creates a `View` for iterating entities with specific components, scoped to
the domain of `THandle`:

```cpp
for (auto [entity, transform, velocity] :
     world.view<GameHandle, TransformComponent, VelocityComponent>()) {
    transform->position += velocity->direction * deltaTime;
}
```

### cloneEntity\<THandle\>(source)

Creates a new entity in the same domain and clones all components from `source`:

```cpp
auto clone = world.cloneEntity<GameHandle>(player.handle());
```

## Use Cases

### Single-Domain World

The simplest setup uses one EntityManager:

```cpp
using GameEM = EntityManager<GameHandle, GameReg, 4096>;

TypedHandleWorld<GameEM> world;
auto entity = world.addEntity<GameHandle>();
```

### Multi-Domain World

Separate domains keep their entity IDs, versions and component type IDs
independent:

```cpp
using GameEM  = EntityManager<GameHandle, GameReg, 4096>;
using UiEM    = EntityManager<UiHandle,   UiReg,   512>;
using AudioEM = EntityManager<AudioHandle, AudioReg, 256>;

TypedHandleWorld<GameEM, UiEM, AudioEM> world;

auto player = world.addEntity<GameHandle>();
auto hud    = world.addEntity<UiHandle>();
auto sfx    = world.addEntity<AudioHandle>();
```

### Domain-Specific Component Type IDs

Because `ComponentTypeId` and `ComponentOpsRegistry` are templated on
`THandle`, each domain has its own independent type-ID counter and lifecycle
registry. A `TransformComponent` in the game domain and a
`TransformComponent` in the UI domain receive separate type IDs — even though
they are the same C++ type.

```cpp
// Game domain uses ComponentTypeId<GameHandle>
// UI domain uses ComponentTypeId<UiHandle>

// Type registration is also per-domain:
ComponentReflector<GameHandle, GameEM>::registerType<TransformComponent>();
ComponentReflector<UiHandle, UiEM>::registerType<TransformComponent>();
```

## HandleToManager (Implementation Detail)

The `HandleToManager` trait recursively matches a handle type to a manager
index in the parameter pack. It is an implementation detail but useful to
understand:

```cpp
template<typename THandle, typename... TEntityManagers>
struct HandleToManager;

// Recursive case: check if THead::Handle_type matches THandle
template<typename THandle, typename THead, typename ...TTail>
struct HandleToManager<THandle, THead, TTail...> {
    static constexpr size_t value = /* 0 if match, else 1 + recurse */;
};

// Base case: static_assert if no match found
template<typename THandle>
struct HandleToManager<THandle> {
    static_assert(sizeof(THandle) == 0,
        "No EntityManager found for the given handle type.");
};
```

## Thread Safety

`TypedHandleWorld` is **not thread-safe**. Each `EntityManager` and its
underlying registry must be accessed from a single thread or externally
synchronised.

## See Also

- [Entity](entity.md) - The entity facade returned by `addEntity()`
- [EntityManager](entity-manager.md) - Per-domain component storage
- [EntityHandle](entity-handle.md) - Versioned, strongly-typed entity reference
- [View](view.md) - Component-based entity queries
- [EntityResolver](entity-resolver.md) - Handle-to-Entity resolution

