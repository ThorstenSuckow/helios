# helios::gameplay::builder::entity

Factory and prototype classes for constructing Entities with fluent configuration.

## Overview

This submodule contains the core factory pattern implementation for entity creation. The `EntityFactory` provides static methods for creating new entities or cloning existing ones, while `EntityPrototype` enables fluent, callback-based component configuration.

## Key Classes

| Class | Purpose |
|-------|---------|
| `EntityFactory` | Static factory for entity creation and cloning |
| `EntityPrototype` | Internal builder holding all domain-specific builders |

## API

### EntityFactory

```cpp
// Create a new empty Entity
auto prototype = EntityFactory::entity();

// Clone an existing Entity (copies all Cloneable components)
auto prototype = EntityFactory::from(existingEntity);
```

### EntityPrototype

```cpp
prototype
    .withMotion([](MotionBuilder& b) { /* configure motion */ })
    .withRendering([](RenderingBuilder& b) { /* configure rendering */ })
    .withCollision([](CollisionBuilder& b) { /* configure collision */ })
    .withTransform([](TransformBuilder& b) { /* configure transform */ })
    .withUiTransform([](UiTransformBuilder& b) { /* configure UI transform */ })
    .withEffects([](EffectsBuilder& b) { /* configure effects */ })
    .withSpawn([](SpawnBuilder& b) { /* configure spawn */ })
    .withAi([](AiBuilder& b) { /* configure AI */ })
    .withCombat([](CombatBuilder& b) { /* configure weapons */ })
    .withLifecycle([](LifecycleBuilder& b) { /* configure lifecycle */ })
    .withHealth([](HealthBuilder& b) { /* configure health */ })
    .withScoring([](ScoringBuilder& b) { /* configure scoring */ })
    .withObserver([](ObserverBuilder& b) { /* configure observers */ })
    .make(active);  // Returns std::unique_ptr<Entity>
```

## Cloning Behavior

When using `EntityFactory::from()`, only components implementing the `Cloneable` interface are copied. Components that add dependent components in `onAttach()` will have those dependencies resolved via `getOrAdd()` to prevent duplicates.

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.gameplay.builder.entity.builders` | Domain-specific builders |
| `helios.ecs` | Entity, Component, Cloneable |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::builder::entity
@brief Factory and prototype classes for Entity construction.
@details This submodule contains the core factory pattern implementation for entity creation. EntityFactory provides static methods for creating new entities or cloning existing ones. EntityPrototype enables fluent, callback-based component configuration through domain-specific builders.
</p></details>
