# helios::engine::modules

Domain-specific components and systems built on top of the ECS architecture.

## Overview

This module provides concrete game logic implementations using the composition-based component model. Components are stored in SparseSets and manipulated through the ECS View system.

For gameplay mechanics (bounds, combat, spawn, input, health, scoring), see `helios.gameplay`.

## Submodules

| Module | Purpose |
|--------|---------|
| `ai/` | AI behavior components (chase, seek) |
| `effects/` | Visual effects (spin, fade) |
| `physics/` | Physics simulation (collision, motion) |
| `spatial/` | Transform management (translation, rotation, scale) |
| `scene/` | Scene graph integration |
| `rendering/` | Renderable and model components |
| `ui/` | UI layout, transform, widgets, data binding |

## Architecture

```
helios.engine.modules
├── ai/              # AI behavior
│   └── chase/       # Chase/seek behavior
├── effects/         # Visual effects
│   └── gfx/         # Graphics effects (spin)
├── physics/
│   ├── collision/   # AABB, collision detection, collision state
│   └── motion/      # Movement (Move2D), steering, direction
├── spatial/
│   └── transform/   # Translation, rotation, scale components
├── scene/           # SceneNode integration
├── rendering/
│   ├── model/       # Model AABB component
│   └── renderable/  # Renderable component
└── ui/
    ├── binding/     # Data binding
    ├── layout/      # Anchors, layout
    ├── transform/   # UI positioning
    └── widgets/     # Text, buttons
```

## Key Components

| Component | Purpose |
|-----------|---------|
| `CollisionComponent` | Layer-based collision filtering |
| `CollisionStateComponent` | Per-frame collision state |
| `Move2DComponent` | 2D physics-based movement |
| `SteeringComponent` | Rotation and heading control |
| `TranslationStateComponent` | Position state |
| `ScaleStateComponent` | Scale state |
| `RotationStateComponent` | Rotation state |
| `SceneNodeComponent` | Links entity to scene graph |
| `RenderableComponent` | Visual representation |
| `UiTransformComponent` | Viewport-relative positioning |

## Usage Example

```cpp
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.collision.components.CollisionComponent;
import helios.engine.modules.scene.components.SceneNodeComponent;

// Add domain components
entity.add<Move2DComponent>(speed);
entity.add<CollisionComponent>(layerId);
entity.add<SceneNodeComponent>(sceneNode);

// Query with views
for (auto [go, move, collision, active] : gameWorld->view<
    Move2DComponent,
    CollisionComponent,
    Active
>().whereEnabled()) {
    // Process entities
}
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules
@brief Domain-specific components and systems.
@details This namespace provides concrete game logic implementations built on top of the ECS architecture. It includes AI behavior, visual effects, physics systems (collision, motion), spatial transform components, scene graph integration, rendering components, and UI systems. For gameplay mechanics (combat, spawn, bounds, health, scoring), see `helios::gameplay`.
</p></details>
