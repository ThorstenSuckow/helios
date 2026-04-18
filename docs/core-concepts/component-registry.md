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
helios.engine.bootstrap
├── helios.gameplay.registry
│   ├── scoring.registry
│   ├── health.registry
│   ├── spawn.registry
│   ├── bounds.registry
│   ├── combat.registry
│   ├── damage.registry
│   └── lifecycle.registry
├── helios.registry
│   ├── ai.registry
│   ├── rendering.registry
│   │   ├── renderable.registry
│   │   └── model.registry
│   ├── spatial.registry
│   │   └── transform.registry
│   ├── ui.registry
│   │   ├── widgets.registry
│   │   └── transform.registry
│   ├── scene.registry
│   ├── physics.registry
│   │   ├── collision.registry
│   │   └── motion.registry
│   └── effects.registry
│       └── gfx.registry
└── helios.ecs.registry
```

## Registering New Components

### 1. Create Registry File

Each module with components requires a `registry.ixx` file:

```cpp
// helios/engine/modules/physics/collision/registry.ixx
module;

export module helios.physics.collision.registry;

import helios.ecs.ComponentReflector;
import helios.physics.collision.components;

export namespace helios::physics::collision {

    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::CollisionStateComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::CollisionComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::AabbColliderComponent<typename TEntityManager::Handle_type>>();
    }

}
```

### 2. Update Parent Registry

Add the new sub-module to the parent registry:

```cpp
// helios/engine/modules/physics/registry.ixx
module;

export module helios.physics.registry;

import helios.physics.collision.registry;
import helios.physics.motion.registry;

export namespace helios::physics {

    template<typename TEntityManager>
    inline void registerComponents() {
        helios::physics::collision::registerComponents<TEntityManager>();
        helios::physics::motion::registerComponents<TEntityManager>();
    }

}
```

### 3. Export Components Module

Ensure all components are exported in the `_module.ixx` of the `components` directory:

```cpp
// helios/engine/modules/physics/collision/components/_module.ixx
export module helios.physics.collision.components;

export import helios.physics.collision.components.CollisionComponent;
export import helios.physics.collision.components.CollisionStateComponent;
export import helios.physics.collision.components.AabbColliderComponent;
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
| `void onClone(const T&)` | Called after copy construction during cloning |
| `void onActivate()` | Called when owning GameObject is activated |
| `void onDeactivate()` | Called when owning GameObject is deactivated |

## Bootstrap

Component registration is handled automatically by `bootstrapGameWorld()`:

```cpp
import helios.engine.bootstrap;

int main() {
    // bootstrapGameWorld() calls registerAllComponents() internally
    auto [gameWorldPtr, gameLoopPtr] = helios::engine::bootstrap::bootstrapGameWorld();

    // ... application-specific setup
}
```

`registerAllComponents()` uses `ComponentRegistrar<RegisteredEntityManagers>` to
expand registration across all entity-manager types defined in `EngineWorld`.
The function is idempotent — multiple calls have no effect.

## See Also

- [Component System](component-system.md) - Component system fundamentals
- [Component Lifecycle](component-lifecycle.md) - Lifecycle hooks in detail
- [GameObject](ecs/gameobject.md) - High-level entity wrapper
- [EntityManager](ecs/entity-manager.md) - Low-level entity management



