# View

A **View** provides lightweight, efficient iteration over entities that have specific components. It uses the sparse set architecture to achieve cache-friendly traversal without copying data.

> **Migration note:** `View` has been moved from `helios.engine.ecs` to `helios.core.ecs` and generalised with a `TEntityManager` template parameter. This allows views to work with any domain-specific EntityManager specialisation.

## Overview

Views enable component-based queries using a fluent API:

```cpp
// In a system's update():
void update(UpdateContext& ctx) noexcept {
    // Iterate all entities with Transform and Velocity
    for (auto [entity, transform, velocity] : ctx.view<
        TransformComponent,
        VelocityComponent
    >()) {
        transform->position += velocity->direction * ctx.deltaTime();
    }

    // With filters
    for (auto [entity, health, active] : ctx.view<
        HealthComponent,
        Active
    >().whereEnabled().exclude<Invincible>()) {
        // Process only enabled, non-invincible entities
    }
}
```

## Template Parameters

```cpp
template<typename TEntityManager, typename... Components>
class View;
```

| Parameter | Description |
|-----------|-------------|
| `TEntityManager` | The concrete `EntityManager` specialisation to iterate over. Determines the handle type and component storage. |
| `Components...` | The component types to query for. The first type serves as the "lead" iterator. |

The `TEntityManager` parameter was added when the View was moved to
`helios.core.ecs`, replacing the hard-coded dependency on the engine's
concrete `EntityManager`.

## Architecture

The View uses the **lead set pattern**:

1. The first component type becomes the "lead" iterator
2. For each entity in the lead set, check existence in other sets
3. Apply exclusion and filter predicates
4. Yield matching entities with their components

```
Lead Set (Transform):     [E0, E2, E5, E7, E9]
                           ↓   ↓   ↓   ↓   ↓
Velocity Set:             [E0, E2, __, E7, __]  ← Cross-reference
                           ✓   ✓   ✗   ✓   ✗
                           
Result:                   [E0, E2, E7]
```

## API Reference

### Construction

Views are typically obtained from `TypedHandleWorld` or `UpdateContext`:

```cpp
// Via TypedHandleWorld
auto view = world.view<GameHandle, ComponentA, ComponentB>();

// Via UpdateContext (in a system)
auto view = ctx.view<ComponentA, ComponentB>();
```

Or directly from an `EntityManager`:

```cpp
View<GameEM, ComponentA, ComponentB> view(&entityManager);
```

### Filtering

#### exclude\<T\>()

Excludes entities that have a specific component:

```cpp
for (auto [e, health] : world.view<GameHandle, HealthComponent>()
    .exclude<ShieldComponent>()) {
    // Only unshielded entities
}
```

#### whereEnabled()

Filters to only include entities where all queried components are enabled:

```cpp
for (auto [e, move, active] : world.view<GameHandle,
    Move2DComponent,
    Active
>().whereEnabled()) {
    // Only if move.isEnabled() && active.isEnabled()
}
```

Components without an `isEnabled()` method are assumed enabled.

### Iteration

Views support range-based for loops with structured bindings:

```cpp
for (auto [entity, compA, compB] : view) {
    // entity: Entity<THandle, TEntityManager>
    // compA: ComponentA*
    // compB: ComponentB*
}
```

The tuple contains:
1. `Entity<THandle, TEntityManager>` — wrapper for the entity
2. `Component*...` — pointers to each queried component

## Usage Patterns

### Basic Query

```cpp
for (auto [entity, transform] : ctx.view<TransformComponent>()) {
    transform->setPosition(newPos);
}
```

### Multi-Component Query

```cpp
for (auto [entity, transform, velocity, gravity] : ctx.view<
    TransformComponent,
    VelocityComponent,
    GravityComponent
>()) {
    velocity->velocity += gravity->force * deltaTime;
    transform->position += velocity->velocity * deltaTime;
}
```

### With Active Tag

```cpp
// Common pattern: include Active tag to skip inactive entities
for (auto [entity, health, damage, active] : ctx.view<
    HealthComponent,
    DamageDealerComponent,
    Active
>().whereEnabled()) {
    // Process only active entities
}
```

### Exclusion

```cpp
// Find all enemies without AI (for debugging)
for (auto [entity, enemy] : ctx.view<EnemyComponent>()
    .exclude<AIComponent>()) {
    LOG_WARN("Enemy {} has no AI!", entity.entityHandle().entityId);
}
```

### Chained Filters

```cpp
for (auto [e, h, a] : ctx.view<HealthComponent, Active>()
    .whereEnabled()
    .exclude<Invincible>()
    .exclude<Dead>()) {
    // Enabled, not invincible, not dead
}
```

## Performance Considerations

### Lead Set Selection

The first component type determines iteration order. Choose the **smallest set** as the lead for best performance:

```cpp
// If few entities have RareComponent, put it first
for (auto [e, rare, common] : ctx.view<
    RareComponent,    // Lead - smallest set
    CommonComponent
>()) { }
```

### Sparse Set Iteration

Views iterate the dense storage of the lead set, which is:
- **Cache-friendly** - contiguous memory access
- **No indirection** - direct component access
- **O(n)** - linear in the number of entities with the lead component

### Cross-Reference Cost

Each additional component type adds an O(1) existence check per entity. For views with many component types, this can add up.

## Implementation Details

### Iterator Structure

```cpp
struct Iterator {
    using Entity_type = Entity<typename TEntityManager::Handle_type, TEntityManager>;
    using LeadComponent = std::tuple_element_t<0, std::tuple<Components...>>;
    using LeadIterator  = typename SparseSet<LeadComponent>::Iterator;

    LeadIterator current_;
    LeadIterator end_;
    const View* view_;
    
    bool isValid() const;   // Check all predicates
    void advance();         // Skip to next valid entity
};
```

### Validity Checks

For each entity, the iterator checks:

1. **Entity validity** — Handle is still valid in registry
2. **Include check** — Entity has all required components
3. **Exclude check** — Entity has none of the excluded components
4. **Enabled check** — All components pass `isEnabled()` (if filtered)

## Thread Safety

Views are **not thread-safe**. The underlying EntityManager and SparseSets must not be modified during iteration.

## See Also

- [EntityManager](entity-manager.md) - Component storage
- [SparseSet](../sparse-set.md) - Underlying data structure
- [Entity](entity.md) - Entity wrapper returned by views
- [TypedHandleWorld](typed-handle-world.md) - World-level view creation
- [System](system.md) - Systems that use views for queries
- [Traits](traits.md) - isEnabled() detection
- [Component Lifecycle](../component-lifecycle.md) - isEnabled() and other hooks

