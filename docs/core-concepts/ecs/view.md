# View

A **View** provides lightweight, efficient iteration over entities that have specific components. It uses the sparse set architecture to achieve cache-friendly traversal without copying data.

## Overview

Views enable component-based queries using a fluent API:

```cpp
// Iterate all entities with Transform and Velocity
for (auto [entity, transform, velocity] : gameWorld->view<
    TransformComponent,
    VelocityComponent
>()) {
    velocity->position += velocity->direction * deltaTime;
}

// With filters
for (auto [entity, health, active] : gameWorld->view<
    HealthComponent,
    Active
>().whereEnabled().exclude<Invincible>()) {
    // Process only enabled, non-invincible entities
}
```

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

Views are typically obtained from `GameWorld`:

```cpp
auto view = gameWorld->view<ComponentA, ComponentB, ComponentC>();
```

Or directly from `EntityManager`:

```cpp
View<ComponentA, ComponentB> view(&entityManager);
```

### Filtering

#### exclude<T>()

Excludes entities that have a specific component:

```cpp
// Skip entities with Shield component
for (auto [e, health] : world->view<HealthComponent>()
    .exclude<ShieldComponent>()) {
    // Only unshielded entities
}
```

#### whereEnabled()

Filters to only include entities where all queried components are enabled:

```cpp
for (auto [e, move, active] : world->view<
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
    // entity: GameObject
    // compA: ComponentA*
    // compB: ComponentB*
}
```

The tuple contains:
1. `GameObject` - wrapper for the entity
2. `Component*...` - pointers to each queried component

## Usage Patterns

### Basic Query

```cpp
for (auto [entity, transform] : world->view<TransformComponent>()) {
    transform->setPosition(newPos);
}
```

### Multi-Component Query

```cpp
for (auto [entity, transform, velocity, gravity] : world->view<
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
for (auto [entity, health, damage, active] : world->view<
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
for (auto [entity, enemy] : world->view<EnemyComponent>()
    .exclude<AIComponent>()) {
    LOG_WARN("Enemy {} has no AI!", entity.entityHandle().entityId);
}
```

### Chained Filters

```cpp
for (auto [e, h, a] : world->view<HealthComponent, Active>()
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
for (auto [e, rare, common] : world->view<
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
    LeadIterator current_;  // Points into lead SparseSet
    LeadIterator end_;
    const View* view_;
    
    bool isValid() const;   // Check all predicates
    void advance();         // Skip to next valid entity
};
```

### Validity Checks

For each entity, the iterator checks:

1. **Entity validity** - Handle is still valid in registry
2. **Include check** - Entity has all required components
3. **Exclude check** - Entity has none of the excluded components
4. **Enabled check** - All components pass `isEnabled()` (if filtered)

## Thread Safety

Views are **not thread-safe**. The underlying EntityManager and SparseSets must not be modified during iteration.

## See Also

- [EntityManager](entity-manager.md) - Component storage
- [SparseSet](../sparse-set.md) - Underlying data structure
- [GameObject](gameobject.md) - Entity wrapper returned by views
- [System](system.md) - Systems that use views for queries
- [Traits](traits.md) - isEnabled() detection
- [Component Lifecycle](../component-lifecycle.md) - isEnabled() and other hooks


