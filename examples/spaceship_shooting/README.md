# Spaceship Shooting Example

This example extends the spaceship control demo with a complete twin-stick shooting system, demonstrating the projectile pooling system, aiming components, and 2D collision detection via AABB.

## Features

- **Twin-Stick Shooter Controls** - Left stick for movement, right stick for aiming and shooting
- **Component-Based Architecture** - Full integration with helios component/system framework
- **Projectile Pool System** - Efficient projectile management with configurable pool size
- **Level Bounds System** - Arena boundaries with bounce behavior
- **ShootComponent** - Configurable cooldown and projectile speed parameters
- **Aim2DComponent** - Direction tracking and firing frequency management
- **Physics Systems** - Move2DSystem, ScaleSystem, BoundsUpdateSystem
- **Scene Synchronization** - SceneSyncSystem bridges gameplay and rendering
- **Ellipse Shape** - 2D ellipse primitive for projectile rendering
- **Colors Utility** - Standard color palette (`helios::util::Colors`) for material properties

## Building

```bash
cmake -S . -B build
cmake --build build --target spaceship_shooting
```

## Running

```bash
./build/bin/spaceship_shooting
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship / Rotate |
| Right Stick | Aim direction / Fire projectiles |
| ESC | Exit application |

## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point and game loop |
| `SpaceshipWidget.ixx` | ImGui widget for physics parameter tuning |

### Components Used

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links GameObject to scene graph |
| `Move2DComponent` | 2D physics with acceleration, dampening, and rotation |
| `TwinStickInputComponent` | Translates dual analog stick input to movement/aim commands |
| `TransformComponent` | Stores local/world transform state |
| `ScaleComponent` | Unit-based sizing (meters) |
| `Aim2DComponent` | Tracks aim direction and firing frequency |
| `ShootComponent` | Manages projectile firing with cooldown timer |
| `LevelBoundsBehaviorComponent` | Bounce behavior at arena boundaries |
| `AabbColliderComponent` | World-space collision bounds |

### Systems Used

| System | Purpose |
|--------|---------|
| `ProjectilePoolSystem` | Object pool for projectile spawning and recycling |
| `ScaleSystem` | Applies ScaleComponent sizing |
| `Move2DSystem` | Physics simulation (rotation, velocity) |
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `BoundsUpdateSystem` | Updates AABB colliders from transforms |
| `LevelBoundsBehaviorSystem` | Handles boundary collisions |
| `TransformClearSystem` | Clears dirty flags post-frame |
| `ScaleClearSystem` | Clears scale dirty flags |

## Projectile Pool System

The example demonstrates efficient projectile management with systems:

```cpp
// Create projectile renderable with ellipse shape
auto projectile = Ellipse{1.0f, 0.4f, 8};  // width, height, segments
auto projectileMesh = std::make_shared<OpenGLMesh>(projectile, meshConfig);
auto projectilePrototype = std::make_shared<RenderPrototype>(material, projectileMesh);
auto projectileRenderable = std::make_shared<Renderable>(std::move(projectilePrototype));

// Add projectile pool system to game world
gameWorld.add<ProjectilePoolSystem>(std::move(projectileRenderable), 50);

// Register physics and cleanup systems
gameWorld.add<ScaleSystem>();
gameWorld.add<Move2DSystem>();
gameWorld.add<SceneSyncSystem>(scene.get());
gameWorld.add<BoundsUpdateSystem>();
gameWorld.add<LevelBoundsBehaviorSystem>();
gameWorld.add<TransformClearSystem>();
gameWorld.add<ScaleClearSystem>();
```

## Color Palette

The example uses the standard color palette for material properties:

```cpp
import helios.util.Colors;

// Use predefined colors
auto spaceshipMaterial = MaterialProperties(helios::util::Colors::Yellow);
auto gridMaterial = MaterialProperties(helios::util::Colors::Turquoise.withW(0.2f));
auto aimGizmo = MaterialProperties(helios::util::Colors::Magenta);
```

## Aiming and Shooting

The shooting system uses components for clean separation of concerns:

```cpp
// Add aiming and shooting components to spaceship
shipGameObject->add<Aim2DComponent>();
shipGameObject->add<ShootComponent>();

// ShootComponent automatically interacts with ProjectilePoolSystem
// when the right stick is active (via TwinStickInputComponent)
```

## Debug Visualization

Gizmo lines visualize input and movement state:

- **Green Line** - Left stick input scaled by throttle
- **Magenta Line** - Right stick aim direction scaled by frequency
- **Red Line** - Ship velocity direction scaled by speed ratio

## ImGui Widgets

The example includes the same widget setup as spaceship_control:

- `MainMenuWidget` - Application settings (transparency, docking, themes)
- `FpsWidget` - Frame rate display and target FPS control
- `GamepadWidget` - Real-time gamepad state and deadzone settings
- `LogWidget` - Scrollable log console with scope filtering
- `CameraWidget` - Camera parameter control
- `SpaceshipWidget` - Physics parameter tuning for Move2DComponent

## Technical Notes

### Arena Collision

Projectiles and the spaceship are constrained to the arena bounds defined via the Level system:

```cpp
// Create level with arena bounds
auto level = std::make_unique<Level>(&(scene->root()));
level->setBounds(
    aabb{-72.5f, -47.5f, 0.0f, 72.5f, 47.5f, 0.0f},
    Unit::Meter
);
gameWorld.setLevel(std::move(level));

// Spaceship bounces off arena edges via LevelBoundsBehaviorComponent
shipGameObject->add<LevelBoundsBehaviorComponent>();
```

> **Note:** The grid's Z-index is at 0 (min/max), which works for 2D gameplay but should be made more flexible for 3D scenarios.

### Unit System

Object sizes use the helios unit system (1 hu = 1 meter) via `ScaleComponent`:

```cpp
// Set spaceship size to 5x5 meters
shipGameObject->add<ScaleComponent>(5.0f, 5.0f, 0.0f, Unit::Meter);
```

