# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

#### ECS Architecture (Sparse-Set Refactor)
- `EntityHandle` struct for versioned entity references, replacing `GUID`-based identification
- `EntityId` and `VersionId` strongly-typed identifiers for entity management
- `EntityRegistry` as the single source of truth for entity lifecycle (create, destroy, validate)
- `EntityTombstone` sentinel for marking invalid sparse array indices
- `SparseSet<T>` generic container with paged sparse arrays, swap-and-pop removal, and dense iteration
- `EntityManager` for centralized entity and component management with validation
- `ComponentReflector` and `ComponentOpsRegistry` for runtime component lifecycle management
- `Active` / `Inactive` tag components for entity activation state
- Compile-time traits for ECS component lifecycle hooks (clone, reset, enable/disable)
- `HierarchyComponent` for parent-child entity relationships
- `HierarchyPropagationSystem` for propagating transforms through entity hierarchies
- `helios.engine.bootstrap` module with per-module component registry files for centralized type registration
- `GameObject::remove<T>()` method for component removal
- `View::whereEnabled()` filter for iterating only over entities with `Active` component

#### Resource Registry (#217)
- `ResourceRegistry` type-indexed service locator for Managers, CommandBuffers, and CommandHandlers with O(1) lookup via `ResourceTypeId`
- `ResourceTypeId` compile-time type identifier for engine resources (Managers, CommandBuffers, CommandHandlers)
- `ErasedUnique` type-erased unique ownership wrapper (16 bytes: `void*` + deleter) in `helios.core.memory`
- `helios.core.memory` module for low-level memory utilities
- `GameWorld::resourceRegistry()` accessor for direct resource registration and lookup
- `GameWorld::registerManager<T>()` convenience method replacing `addManager<T>()`
- `GameWorld::registerCommandHandler<T>()` for non-owning handler registration
- `GameWorld::registerResource<T>()` for arbitrary owning resource registration
- `GameWorld::manager<T>()` accessor replacing `getManager<T>()`
- `GameWorld::entityManager()` public accessor

#### Compile-Time Typed Command System (#217)
- `TypedCommandBuffer<...CommandTypes>` compile-time typed buffer with per-type queues stored in `std::tuple<std::vector<CommandType>...>`
- `TypedCommandHandler<T>` type-safe handler interface for receiving commands of a specific type
- `CommandHandler` abstract base for type-erased handler storage in ResourceRegistry
- `CommandBuffer` refactored to abstract base class with `flush(UpdateContext&)` and `clear()` interface
- `EngineCommandBuffer` concrete facade pre-configured with all engine command types (Aim2D, Shoot, Move2D, Steering, UpdateScore, ScheduledSpawnPlan, Spawn, Despawn, StateCommand<GameState>, StateCommand<MatchState>, UiAction, TimerControl, WorldLifecycle)
- `ExecutableCommand` concept for commands that can self-execute via `execute(UpdateContext&)` method
- `CommandTypeId` compile-time type identifier for command types
- `WorldLifecycleCommand` and `WorldLifecycleManager` for deferred world reset operations

#### Entity Resolution (#224)
- `EntityResolver` callable struct for resolving `EntityHandle` to `GameObject` via `EntityManager`
- `UpdateContext::find(EntityHandle)` for entity lookup without direct `GameWorld` access

#### Bootstrap Factory (#217)
- `helios::engine::bootstrap::makeGameWorld()` factory returning pre-configured `GameWorld` + `GameLoop` pair with `EngineCommandBuffer`, `WorldLifecycleManager`, `GameStateManager`, `MatchStateManager`, and tracked states

#### State Management (#222)
- `helios.engine.state` module with generic, template-based state management
- Generic `StateManager<TState>` template base class eliminating code duplication between `GameStateManager` and `MatchStateManager`
- `StateTransitionId<T>` trait specialization mechanism via `helios.engine.state.Bindings`
- `StateTransitionContext<T>` for type-safe state transitions
- `StateToIdMap<State, Id>` and `StateToIdMapPair<Lft, Rgt, Id>` for state-to-typed-ID mapping (#225)
- `GameState` bitmask enum with `Undefined`, `Start`, `Running`, `Paused`, `GameOver`, `Any` states
- `MatchState` bitmask enum with `Undefined`, `WaitingForPlayers`, `Playing`, `GameOver`, `Any` states
- `GameStateManager` and `MatchStateManager` for hierarchical state machine management
- `GameFlowSystem` for managing game state transitions
- `MatchFlowSystem` for managing match state transitions with previous-state tracking
- `DefaultGameStateTransitionGuards` and `DefaultMatchStateTransitionGuards` for state rules
- `LambdaGameStateListener` and `LambdaMatchStateListener` for callback-based state observation
- `MatchStateComponent` for tracking per-entity match state and transition context
- `GameStateTransitionId` and `MatchStateTransitionId` for named transitions
- `Session` class for managing game/match state, player entity, and active viewport IDs
- `Session::trackState<T>()` for explicit state registration

#### Rendering and Text
- `TextRenderer` abstract class for text rendering operations
- `OpenGLGlyphTextRenderer` for FreeType-based text rendering with OpenGL
- `TextRenderable` for high-level text integration in scenes
- `TextRenderPrototype` for shared, immutable text rendering configuration
- `TextRenderCommand` for low-level text render queue operations
- `TextMesh` for text layout and vertex data management
- `TypeSetter` for glyph positioning and line layout
- `FontResourceProvider` abstract interface for font management
- `FreeTypeFontResourceManager` for font loading and glyph caching
- `MeshRenderable` class separating mesh rendering from generic renderables
- `OpenGLMeshRenderer` for dedicated mesh geometry rendering with OpenGL
- `OpenGLEnumMapper` for mapping `PrimitiveType` to OpenGL enums
- `Glyph`, `DrawProperties`, `TextShaderProperties`, `TextShaderPropertiesOverride` data structures
- `FontId` strongly-typed identifier for font families
- Orthographic projection support in `Camera` (`ortho()`)
- `render_text_demo` example demonstrating text rendering pipeline

#### Scene and Viewport
- `SceneId` strongly-typed identifier for scenes
- `SceneToViewportMap` for associating scenes with viewports
- `SceneRenderingSystem` for automated viewport-based scene rendering
- `StateToViewportPolicyUpdateSystem<Lft, Rgt>` for state-driven viewport activation
- `ViewportId` strongly-typed identifier (replacing raw viewport pointers)

#### UI System
- `helios.engine.modules.ui` module for UI components and systems
- `MenuComponent`, `MenuFocusComponent`, `UiStyleComponent` for menu state
- `MenuConfig`, `MenuItemConfig`, `MenuBuilder` for fluent menu configuration
- `MenuDisplaySystem` for state-driven menu visibility management
- `MenuNavigationSystem` for gamepad-based menu navigation
- `UiTransformSystem` with hierarchy support and anchor positioning (`TopLeft`, `BottomLeft`)
- `UiTextComponent` with `renderable()` accessors
- `ActionId` and `MenuId` strongly-typed identifiers
- `UiActionCommand`, `UiActionCommandHandler`, `UiActionCommandManager` for UI interactions
- `UiTextFormatSystem` for batched text formatting, extracted from `UiTextComponent::update()` (#230)

#### Score Observer (#229)
- `ScoreObserverComponent` and `MaxScoreObserverComponent` now store only `ScorePoolRevision` instead of full snapshots

#### Builder DSL
- `SpawnSystemFactory` fluent builder API for spawn pool, profile, and scheduler configuration
- `.pool().profile().scheduledBy().commit()` chain for declarative spawn setup
- `.commitCyclic<N>()` for cyclic scheduler registration
- `.placer()`, `.initializer()`, `.axisPlacement()`, `.moveInitializer()`, `.randomDirectionInitializer()` profile helpers
- `.emitterPlacement()` for emitter-relative spawn positioning
- `.timerCondition()`, `.timerWithAvailabilityCondition()`, `.fixedAmount()` rule helpers
- `MenuBuilder` for fluent menu and menu item configuration

#### Typed Identifiers
- `StrongId<Tag>` generic module for strongly-typed identifiers with FNV-1a hashing
- `PrefabId` for prefab identity management in object pooling
- `PrefabIdComponent` for tagging GameObjects as prefab instances
- `ViewportId`, `SceneId`, `FontId`, `ActionId`, `MenuId` as `StrongId` specializations
- `StateTypeId` for unique state type identification at runtime

#### Other
- `GamepadState` button state tracking and input checks
- `Manager::reset()` interface for state initialization
- `SpawnInitializer::reset()` / `SpawnScheduler::reset()` for resetting internal states
- `GameObjectPool` span accessors for active and inactive GameObjects
- `Random` initial seed storage and `reset()` functionality
- `ScorePool` and `ScorePoolManager` reset functionality
- `vec2ui` type for unsigned integer 2D vectors
- `ortho()` function for orthographic projection matrices
- FreeType library integration for text rendering

#### Documentation
- `docs/core-concepts/resource-registry.md` — comprehensive documentation for ResourceRegistry including custom Managers, custom CommandBuffers, and plain resources
- Updated `docs/core-concepts/command-system.md` to reflect compile-time typed architecture
- Updated `docs/core-concepts/gameloop-architecture.md` for ResourceRegistry integration
- Updated `docs/core-concepts/object-pooling.md` with ResourceRegistry cross-reference

### Changed
- **BREAKING**: Replaced `GUID`-based entity identification with `EntityHandle` (index + version)
- **BREAKING**: `GameObject` is now a lightweight facade (~16 bytes, safe to copy by value)
- **BREAKING**: `Component` is no longer a superclass; components use compile-time traits instead
- **BREAKING**: All component `explicit` copy constructors removed (aggregate-style components)
- **BREAKING**: `GameObjectSpawnSystem` now takes `SpawnManager&` instead of `vector<unique_ptr<SpawnScheduler>>`
- **BREAKING**: `GameLoop::Phase::addPass()` now requires state template parameter: `.addPass<GameState>(GameState::Any)`
- **BREAKING**: `Viewport` constructor requires `ViewportId` parameter
- **BREAKING**: `Scene` constructor requires `SceneId` parameter
- **BREAKING**: `SceneSyncSystem` takes `SceneToViewportMap&` instead of `Scene*`
- **BREAKING**: `Renderable` refactored; `RenderableComponent` now uses `MeshRenderable*` (raw pointer)
- **BREAKING**: `SnapshotItem` uses raw pointer for `Renderable` instead of `weak_ptr`
- **BREAKING**: `RenderPass` uses non-owning pointers; manual rendering loop replaced by `SceneRenderingSystem`
- **BREAKING**: `helios.rendering.model` module renamed to `helios.rendering.material`
- **BREAKING**: `helios.rendering.mesh` module restructured
- **BREAKING**: `makeRenderCommand` emits directly to render queue (no intermediate `RenderCommand`)
- **BREAKING**: `ViewportId` replaces raw viewport references in state-to-viewport policy
- **BREAKING**: `GameObjectPoolConfig` now uses `PrefabId` instead of `EntityHandle` for prefab identification
- **BREAKING**: `CommandBuffer` is now an abstract base class; the concrete implementation is `EngineCommandBuffer` (#217)
- **BREAKING**: Removed `TargetedCommand`, `WorldCommand`, and all command base classes — commands are now plain structs (value types) (#217)
- **BREAKING**: Removed all command dispatchers (`WorldCommandDispatcher`, `TargetedCommandDispatcher`, `TypedWorldCommandDispatcher`, `TypedTargetedCommandDispatcher`, `SpawnCommandDispatcher`, `DespawnCommandDispatcher`, `ScheduledSpawnPlanCommandDispatcher`, `UiActionCommandDispatcher`, `ScoreCommandDispatcher`, `StateCommandDispatcher`, `TimerCommandDispatcher`) — replaced by `TypedCommandHandler<T>` registered via `ResourceRegistry` (#217)
- **BREAKING**: `GameWorld::addManager<T>()` replaced by `GameWorld::registerManager<T>()` (#217)
- **BREAKING**: `GameWorld::getManager<T>()` (returning pointer) replaced by `GameWorld::manager<T>()` (returning reference) (#217)
- **BREAKING**: `Manager::flush()` no longer takes `GameWorld&` parameter — uses `UpdateContext&` only (#224)
- **BREAKING**: `Manager::onAdd()` removed — initialization logic moved to `Manager::init()` (#217)
- **BREAKING**: `Manager::gameWorld_` protected member removed (#217, #224)
- **BREAKING**: `UpdateContext` no longer exposes full `GameWorld` reference — provides `resourceRegistry()`, `session()`, `find()`, `commandBuffer()`, and `level()` instead (#224)
- **BREAKING**: `UpdateContext::commandBuffer()` now returns `EngineCommandBuffer&` from ResourceRegistry instead of `CommandBuffer&` from GameLoop (#217)
- **BREAKING**: `GameLoop` no longer owns `CommandBuffer` — it is owned by `GameWorld::ResourceRegistry` (#217)
- **BREAKING**: `GameLoop::commandBuffer()` accessor removed (#217)
- **BREAKING**: `GameLoop::update()` signature changed to `update(GameWorld&, float, InputSnapshot&, span<ViewportSnapshot>)` (#224)
- **BREAKING**: `GameWorld` removed all domain-specific handler registrations (`registerScoreCommandHandler`, `registerSpawnCommandHandler`, `registerStateCommandHandler`, `registerTimerCommandHandler`, `registerUiActionCommandHandler`) — all replaced by generic `registerCommandHandler<T>()` + ResourceRegistry (#217)
- **BREAKING**: `GameWorld` removed `SpawnCommandHandlerRegistry`, `scoreCommandHandler_`, `timerCommandHandler_`, `uiActionCommandHandler_`, `stateCommandHandlers_` members (#217)
- **BREAKING**: `Aim2DCommand` and `ShootCommand` no longer inherit from `TargetedCommand` — they are self-executing commands carrying an `EntityHandle` and resolving entities via `UpdateContext::find()` (#217, #224)
- **BREAKING**: `GameStateManager` and `MatchStateManager` now derive from generic `StateManager<TState>` template (#222)
- **BREAKING**: `CollisionBehavior::Despawn` renamed to `CollisionBehavior::PassEvent`
- `GameWorld::level()` returns pointer instead of reference
- `ChaseSystem` now only considers active entities
- UI offsets replace margins in `UiTransformSystem`
- `TextRenderable` uses `TextMesh` for text management
- `OpenGLGlyphTextRenderer` uses `FontResourceProvider` for glyph rendering
- `GLFWFactory` uses `FreeTypeFontResourceManager`
- `GameStateManager` and `MatchStateManager` are now auto-registered by `bootstrap::makeGameWorld()` with default transition rules
- `SpawnManager::init()` registers `TypedCommandHandler<SpawnCommand>`, `TypedCommandHandler<DespawnCommand>`, and `TypedCommandHandler<ScheduledSpawnPlanCommand>` via `GameWorld::registerCommandHandler<T>()`
- `ScorePoolManager`, `TimerManager`, `UiActionCommandManager` register their `TypedCommandHandler` specializations during `init()`
- `ScoreObserverComponent` / `MaxScoreObserverComponent` store revision only; UI-binding systems read values from pool directly (#229)
- `UiTextComponent` setters (`setDouble()`, `setText()`) no longer call `update()` — formatting is handled by `UiTextFormatSystem` (#230)
- Commands are flushed in deterministic template parameter order (combat → motion → scoring → spawn → state → UI → timing → lifecycle)
- All examples updated to use `bootstrap::makeGameWorld()` factory and `registerManager<T>()` API
- `helios.engine.Bootstrap` module renamed to `helios.engine.bootstrap` (lowercase)
- `NumberFormatComponent::format()` now requires explicit template parameter: `format<double>()`
- `scoring_demo` refactored into separate module files (`IdConfig`, `ArenaConfig`, `SpawnConfiguration`, `EnemyPrefabs`, `MenuConfiguration`)

### Removed
- `TargetedCommand` base class and `WorldCommand` base class (#217)
- `TargetedCommandDispatcher`, `WorldCommandDispatcher`, `TypedTargetedCommandDispatcher`, `TypedWorldCommandDispatcher` (#217)
- `SpawnCommandDispatcher`, `DespawnCommandDispatcher`, `ScheduledSpawnPlanCommandDispatcher` (#217)
- `UiActionCommandDispatcher` (#217)
- `helios.engine.runtime.spawn.dispatcher` module (#217)
- `SpawnCommandHandler` interface and `SpawnCommandHandlerRegistry` (#217)
- `ScoreCommandHandler`, `TimerCommandHandler`, `StateCommandHandler`, `TypedStateCommandHandler` interfaces (#217)
- `UiActionCommandHandler` interface (#217)
- `Manager::gameWorld_` protected member (#224)
- `Manager::onAdd()` virtual method (#217)
- `GameWorld::gameObjects_` legacy `unordered_map` storage (#217)

### Fixed
- Camera height parameter in resize calculation
- `zFar` assertion in camera setup
- Move operation incorrectly applied to pointer
- Font cache lookup assertion message
- VAO reset timing issue in OpenGL rendering
- Duplicate import of `RotationStateComponent` in `LevelBoundsBehaviorSystem`
- `GameObjectPoolRegistry::has()` now uses `contains()` instead of `find() != end()`

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
- **BREAKING**: `GameWorld` no longer manages System instances - use `GameLoop::phase().addPass().addSystem<T>()`
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
- **BREAKING**: `SceneNodeComponent::setSize()` removed - use `ScaleStateComponent` instead
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
