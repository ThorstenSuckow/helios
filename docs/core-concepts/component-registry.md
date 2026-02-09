# Component Registry System

The **Component Registry System** enables modular registration of ECS components for runtime reflection. Each module manages its own components through a dedicated `registry.ixx` file.

## Overview

Components must be registered with the `ComponentReflector` to support the following features:

- **Cloning:** Copying components when spawning from prefabs
- **Lifecycle Callbacks:** `onAcquire()`, `onRelease()`, `onRemove()`
- **Enable/Disable:** Activating and deactivating components at runtime

## Architecture

The registry structure is organized hierarchically:

```
helios.engine.Bootstrap
├── helios.engine.mechanics.registry
│   ├── scoring.registry
│   ├── health.registry
│   ├── spawn.registry
│   ├── bounds.registry
│   ├── combat.registry
│   ├── damage.registry
│   └── lifecycle.registry
└── helios.engine.modules.registry
    ├── ai.registry
    ├── rendering.registry
    │   ├── renderable.registry
    │   └── model.registry
    ├── spatial.registry
    │   └── transform.registry
    ├── ui.registry
    │   ├── widgets.registry
    │   └── transform.registry
    ├── scene.registry
    ├── physics.registry
    │   ├── collision.registry
    │   └── motion.registry
    └── effects.registry
        └── gfx.registry
```

## Registering New Components

### 1. Create Registry File

Each module with components requires a `registry.ixx` file:

```cpp
// helios/engine/modules/physics/collision/registry.ixx
module;

export module helios.engine.modules.physics.collision.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.physics.collision.components;

export namespace helios::engine::modules::physics::collision {

    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::CollisionStateComponent>();
        R::registerType<components::CollisionComponent>();
        R::registerType<components::AabbColliderComponent>();
    }

}
```

### 2. Update Parent Registry

Add the new sub-module to the parent registry:

```cpp
// helios/engine/modules/physics/registry.ixx
module;

export module helios.engine.modules.physics.registry;

import helios.engine.modules.physics.collision.registry;
import helios.engine.modules.physics.motion.registry;

export namespace helios::engine::modules::physics {

    inline void registerComponents() {
        helios::engine::modules::physics::collision::registerComponents();
        helios::engine::modules::physics::motion::registerComponents();
    }

}
```

### 3. Export Components Module

Ensure all components are exported in the `_module.ixx` of the `components` directory:

```cpp
// helios/engine/modules/physics/collision/components/_module.ixx
export module helios.engine.modules.physics.collision.components;

export import helios.engine.modules.physics.collision.components.CollisionComponent;
export import helios.engine.modules.physics.collision.components.CollisionStateComponent;
export import helios.engine.modules.physics.collision.components.AabbColliderComponent;
```

## Component Requirements

### Move/Copy Operators

Components **must** have a `noexcept` move constructor, as `std::vector` will otherwise use the copy constructor during reallocation:

```cpp
class MyComponent {
public:
    MyComponent() = default;
    
    // Copy
    MyComponent(const MyComponent&) = default;
    MyComponent& operator=(const MyComponent&) = default;
    
    // Move - noexcept is CRITICAL!
    MyComponent(MyComponent&&) noexcept = default;
    MyComponent& operator=(MyComponent&&) noexcept = default;
};
```

> **Important:** Without `noexcept` on the move constructor, `std::vector` may create unexpected copies during reallocation, leading to bugs (e.g., duplicate SceneNodes).

### Optional Lifecycle Hooks

Components can implement the following methods:

| Method | Description |
|--------|-------------|
| `void onAcquire()` | Called when GameObject is acquired from pool |
| `void onRelease()` | Called when GameObject is returned to pool |
| `bool onRemove()` | Called before removal, `false` prevents removal |
| `void enable()` | Enables the component |
| `void disable()` | Disables the component |

## Bootstrap

Registration is called once at engine startup:

```cpp
#include <helios/engine/bootstrap.ixx>

int main() {
    // Must be called before creating GameObjects
    helios::engine::bootstrap::registerAllComponents();
    
    // ... engine initialization
}
```

The `registerAllComponents()` function is idempotent - multiple calls have no effect.

## See Also

- [Component System](component-system.md) - Component system fundamentals
- [Component Lifecycle](component-lifecycle.md) - Lifecycle hooks in detail
- [GameObject](ecs/gameobject.md) - High-level entity wrapper
- [EntityManager](ecs/entity-manager.md) - Low-level entity management



