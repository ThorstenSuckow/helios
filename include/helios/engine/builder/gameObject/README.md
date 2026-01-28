# helios::engine::builder::gameObject

Factory and prototype classes for constructing GameObjects with fluent configuration.

## Overview

This submodule contains the core factory pattern implementation for entity creation. The `GameObjectFactory` provides static methods for creating new entities or cloning existing ones, while `GameObjectPrototype` enables fluent, callback-based component configuration.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObjectFactory` | Static factory for entity creation and cloning |
| `GameObjectPrototype` | Internal builder holding all domain-specific builders |

## API

### GameObjectFactory

```cpp
// Create a new empty GameObject
auto prototype = GameObjectFactory::gameObject();

// Clone an existing GameObject (copies all Cloneable components)
auto prototype = GameObjectFactory::from(existingGameObject);
```

### GameObjectPrototype

```cpp
prototype
    .withMotion([](MotionBuilder& b) { /* configure motion */ })
    .withRendering([](RenderingBuilder& b) { /* configure rendering */ })
    .withCollision([](CollisionBuilder& b) { /* configure collision */ })
    .withTransform([](TransformBuilder& b) { /* configure transform */ })
    .withEffects([](EffectsBuilder& b) { /* configure effects */ })
    .withSpawn([](SpawnBuilder& b) { /* configure spawn */ })
    .withAi([](AiBuilder& b) { /* configure AI */ })
    .withShooting([](ShootingBuilder& b) { /* configure weapons */ })
    .withLifecycle([](LifecycleBuilder& b) { /* configure lifecycle */ })
    .make(active);  // Returns std::unique_ptr<GameObject>
```

## Cloning Behavior

When using `GameObjectFactory::from()`, only components implementing the `Cloneable` interface are copied. Components that add dependent components in `onAttach()` will have those dependencies resolved via `getOrAdd()` to prevent duplicates.

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.builder.gameObject.builders` | Domain-specific builders |
| `helios.engine.ecs` | GameObject, Component, Cloneable |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::builder::gameObject
@brief Factory and prototype classes for GameObject construction.
@details This submodule contains the core factory pattern implementation for entity creation. GameObjectFactory provides static methods for creating new entities or cloning existing ones. GameObjectPrototype enables fluent, callback-based component configuration through domain-specific builders.
</p></details>
