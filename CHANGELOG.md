# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.4.0-milestone4] - 2026-01-29

### Added
- `helios.engine.ecs` module with base classes (`Component`, `GameObject`, `System`, `Updatable`)
- `helios.engine.ecs.query` submodule with `GameObjectFilter` and `GameObjectView`
- `helios.engine.runtime` aggregate module for runtime infrastructure
- `helios.engine.runtime.world` submodule with `GameWorld`, `Level`, `UpdateContext`, `SystemRegistry`, `Manager`
- `helios.engine.runtime.pooling` submodule with `GameObjectPool`, `GameObjectPoolManager`, `GameObjectPoolRegistry`, `GameObjectPoolConfig`, `GameObjectPoolSnapshot`
- `helios.engine.runtime.gameloop` submodule with `GameLoop`, `Phase`, `Pass`
- `helios.engine.runtime.messaging` for commands and events
- `helios.engine.runtime.spawn` submodule for spawn infrastructure:
  - `SpawnManager` for processing spawn/despawn commands
  - `SpawnProfile` for spawn configuration (pool, placer, initializer)
  - `SpawnScheduler` abstract base for rule-based spawn scheduling
  - `DefaultSpawnScheduler` for evaluating all rules each frame
  - `CyclicSpawnScheduler<N>` for round-robin rule evaluation
  - `RuleProcessor` and `DefaultRuleProcessor` for spawn rule processing
  - `SpawnRule`, `SpawnCondition`, `SpawnRuleState` for policy layer
  - `SpawnConditionAll` for composite AND conditions
  - `TimerSpawnCondition` for interval-based spawning
  - `RequestedAmountIsAvailableCondition` for pool availability checks
  - `SpawnAmountProvider`, `FixedSpawnAmount`, `SpawnAmountByCallback` for amount configuration
  - `SpawnPlacer`, `SpawnInitializer` interfaces for spawn behaviors
  - `RandomSpawnPlacer`, `EmitterSpawnPlacer`, `AxisSpawnPlacer`, `DistributedSpawnPlacer<N>` for positioning
  - `EmitterInitializer`, `MoveInitializer`, `DelayedComponentEnablerInitializer` for entity initialization
  - `SpawnCommand`, `DespawnCommand`, `ScheduledSpawnPlanCommand` for command pipeline
  - `SpawnCommandDispatcher`, `DespawnCommandDispatcher`, `ScheduledSpawnPlanCommandDispatcher`
  - `SpawnContext`, `EmitterContext` for spawn state
  - `SpawnPlanCommandExecutedEvent` for frame events
  - `SpawnCommandHandler`, `SpawnCommandHandlerRegistry` for handler management
- `helios.engine.mechanics.lifecycle` for entity lifecycle management:
  - `DelayedComponentEnabler` component for deferred activation
  - `DelayedComponentEnablerSystem` for processing activation timers
- `helios.engine.builder.gameObject.builders.LifecycleBuilder` for lifecycle configuration
- `helios.engine.builder.gameObject.builders.configs.LifecycleConfig` for deferred enablement
- `GridCollisionDetectionSystem` with uniform 3D spatial partitioning
- `HitPolicy` enum (`OneHit`, `All`) for collision count control
- `helios.engine.mechanics` for gameplay-specific systems (bounds, combat, spawn, input)
- `helios.engine.modules` for domain-agnostic subsystems (physics, spatial, scene, rendering, pool)
- `SpawnedByProfileComponent` for tracking entity spawn origin
- `ProjectileSpawnSystem` for combat projectile spawning
- `SpawnProfileId`, `SpawnRuleId`, `GameObjectPoolId` type-safe identifiers with FNV-1a string hashing
- `helios.core.algorithms` module with `fnv1a_hash` compile-time hash function
- `collision_detection` example demonstrating grid collision and spawn patterns
- `enemy_spawn` example demonstrating spawn system
- Core concepts documentation for spawn system, collision detection, and lifecycle management

### Changed
- **BREAKING**: Reorganized `helios.engine` into distinct submodules: `core`, `ecs`, `runtime`, `modules`, `mechanics`, `tooling`
- **BREAKING**: Systems are now registered with `GameLoop` phases/passes instead of `GameWorld`
- **BREAKING**: `GameWorld` no longer manages System instances — use `GameLoop::phase().addPass().addSystem<T>()`
- **BREAKING**: Spawn infrastructure moved from `mechanics/spawn` to `runtime/spawn`
- **BREAKING**: Removed `GameObjectFactory`, `GameObjectPoolFacade`, `PoolRequestHandler`, `PoolRequestHandlerRegistry`
- Moved ECS base classes from `helios.engine.game` to `helios.engine.ecs`
- Moved world management classes to `helios.engine.runtime.world`
- Moved pooling infrastructure to `helios.engine.runtime.pooling`
- Moved messaging infrastructure to `helios.engine.runtime.messaging`
- `helios.engine.game` is now a facade module re-exporting common game development types
- `helios.engine._module.ixx` updated to export `core`, `ecs`, `game`, `runtime`, `tooling` modules
- All aggregate `_module.ixx` files now only export their direct submodules (clean hierarchy)
- `SystemRegistry` is now internal to the `GameLoop` pass structure
- Updated core concepts documentation with spawn system references

## [0.3.0-milestone3] - 2025-12-25

### Added
- `ComposeTransformComponent` for independent transform management (local/world)
- `ScaleStateComponent` for unit-based sizing with dirty flag tracking
- `AabbColliderComponent` for world-space bounding boxes
- `ModelAabbComponent` for original model AABB storage
- `LevelBoundsBehaviorComponent` for configurable boundary reactions (bounce, restitution)
- Automatic AABB capture from SceneNode meshes via `onAttach()`
- `Move2DSystem` for 2D physics with rotation, velocity integration, and dampening
- `ScaleSystem` for applying unit-based scaling from `ScaleStateComponent`
- `BoundsUpdateSystem` for updating AABB colliders from world transforms
- `LevelBoundsBehaviorSystem` for boundary collisions with bounce behavior
- `SceneSyncSystem` for synchronizing gameplay transforms with scene graph
- `TransformClearSystem` for clearing dirty flags at end of frame
- `Level` class with configurable world bounds and AABB-based arena boundaries
- `Aim2DComponent` for direction tracking and firing frequency
- `ShootComponent` for projectile firing with configurable cooldown and speed
- `Ellipse` 2D shape primitive for projectile and particle rendering
- `Colors` struct with comprehensive color palette as `vec4f` (RGBA)
- `LogWidget`: "None" scope option to completely disable logging (default)
- `GamepadWidget`: Settings panel with side-by-side stick configuration
- `spaceship_shooting` demo showcasing twin-stick shooter mechanics

### Changed
- **BREAKING**: `SceneNodeComponent::setSize()` removed — use `ScaleStateComponent` instead
- **BREAKING**: `GameWorld` constructor no longer requires `Scene*` parameter
- **BREAKING**: `Move2DComponent::position()` removed (transforms managed by `ComposeTransformComponent`)
- **BREAKING**: `Move2DComponent::rotationAngle()` renamed to `currentRotationAngle()`
- `SceneNodeComponent::onAttach()` now automatically captures AABB from SceneNode mesh
- Systems must be explicitly registered for game logic


### Fixed
- Forward declaration conflicts between Component and GameObject modules
- Circular dependency in module imports resolved via shared headers


## [0.2.0-milestone2] - 2025-12-16

### Added
- `FramePacer` class for frame rate control with configurable target FPS (#111)
- `FrameStats` structure for frame timing information (frame time, idle time, work time)
- `FpsMetrics` class for frame rate analysis and statistics
- `Stopwatch` high-resolution timer utility
- `CameraSceneNode` for camera integration into scene hierarchy
- `TransformType` enum for selective transform inheritance (Translation, Rotation, Scale)
- `mat4::decompose()` member function for extracting transform components
- `mat4::transpose()` member function for matrix transposition
- `lookAt()` and `lookAtLocal()` methods for camera orientation
- `onWorldTransformUpdate()` virtual callback for transform change notifications
- Camera-follows-object pattern via scene graph parenting
- `GameObject` base class for game entities with GUID identification
- `GameWorld` container for game object management and updates
- `CommandBuffer` for deferred command execution pattern
- `InputSnapshot` for capturing input state per frame
- `Unit` enum (Meter, Centimeter, Seconds, MilliSeconds)
- `GamepadSettings` class for per-controller configuration
- `DeadzoneStrategy` abstract interface for input normalization
- `RadialDeadzoneStrategy` implementation for circular deadzone handling
- `ImGuiBackend` abstraction for platform backends
- `ImGuiGlfwOpenGLBackend` concrete implementation for GLFW/OpenGL
- `ImGuiOverlay` singleton manager with DockSpace support
- `FpsWidget` for frame rate display and target FPS control
- `GamepadWidget` for real-time gamepad state visualization (#114)
- `LogWidget` for scrollable log console with advanced filtering
- `CameraWidget` for camera parameter control
- `MainMenuWidget` for application settings
- `LogSink` abstract interface with self-registering type identifiers
- `ConsoleSink` for stdout output
- `ImGuiLogSink` for `LogWidget` integration

### Changed
- **BREAKING**: Cameras are now managed via `CameraSceneNode` instead of standalone `Camera` objects
- **BREAKING**: `Viewport` now holds `CameraSceneNode*` instead of `Camera*`
- **BREAKING**: Enum counter entries renamed to `size_` (#34)
- **BREAKING**: `MeshData` merged with `Mesh` (#22)
- View matrix computed from inverse of `CameraSceneNode::worldTransform()`
- Logging system refactored to use self-registering sinks
- `UniformValueMap::float_val` return type refactored (#33)
- `Material` ownership structure improved (#13)

### Fixed
- Potential nullptr dereference in `MaterialData` (#16)


## [0.1.0-milestone1] - 2025-10-21

### Added
- Application layer with event system
- Input manager for keyboard, mouse, and gamepad
- Low-level API subsystems integration (GLFW, GLAD)
- Basic rendering pipeline
- Scene graph with transform hierarchy
- Camera system with projection management
- Material and shader system
- Mesh and geometry handling
- Math library (vectors, matrices, transforms)
- Scoped logger implementation (#8)
- CMake build system with C++23 modules support
- Cross-platform support (Windows, Linux, macOS)
- Unit testing framework with Google Test
- Example applications (simple cube rendering, game controller input)


[Unreleased]: https://github.com/thorstensuckow/helios/compare/v0.4.0-milestone4...HEAD
[0.4.0-milestone4]: https://github.com/thorstensuckow/helios/compare/v0.3.0-milestone3...v0.4.0-milestone4
[0.3.0-milestone3]: https://github.com/thorstensuckow/helios/compare/v0.2.0-milestone2...v0.3.0-milestone3
[0.2.0-milestone2]: https://github.com/thorstensuckow/helios/compare/v0.1.0-milestone1...v0.2.0-milestone2
[0.1.0-milestone1]: https://github.com/thorstensuckow/helios/releases/tag/v0.1.0-milestone1
