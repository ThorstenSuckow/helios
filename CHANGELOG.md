# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]


## [Milestone 2] - 2025-12-16

### Added

#### Engine & Tooling
- `FramePacer` class for frame rate control with configurable target FPS (#111)
- `FrameStats` structure for frame timing information (frame time, idle time, work time)
- `FpsMetrics` class for frame rate analysis and statistics
- `Stopwatch` high-resolution timer utility

#### Scene Graph & Camera System
- `CameraSceneNode` for camera integration into scene hierarchy
- `TransformType` enum for selective transform inheritance (Translation, Rotation, Scale) in `helios.math.transform`
- `mat4::decompose()` member function for extracting transform components
- `mat4::transpose()` member function for matrix transposition
- `transformTypeMatch()` helper for bitmask flag testing
- `lookAt()` and `lookAtLocal()` methods for camera orientation
- `onWorldTransformUpdate()` virtual callback for transform change notifications
- Camera-follows-object pattern via scene graph parenting

#### Game System (`helios.engine.game`)
- `GameObject` base class for game entities with GUID identification
- `GameWorld` container for game object management and updates
- `CommandBuffer` for deferred command execution pattern
- `InputSnapshot` for capturing input state per frame
- `InputHandler` interface for input-to-command translation
- `setSize()` method for unit-based object sizing

#### Units System (`helios.core.units`)
- `Unit` enum (Meter, Centimeter, Seconds, MilliSeconds)
- Conversion utilities between unit types
- Standard unit: 1 Meter = 100 Centimeters (cm)
- Time measurement standard: Seconds

#### Gamepad Input System
- `GamepadSettings` class for per-controller configuration
- `DeadzoneStrategy` abstract interface for input normalization
- `RadialDeadzoneStrategy` implementation for circular deadzone handling
- Per-stick deadzone thresholds (0.0 to 0.9)
- Individual axis inversion (X/Y for left and right sticks)
- `InputAdapter::gamepadSettings()` for runtime configuration access
- `InputManager::inputAdapter()` non-const overload for settings modification

#### ImGui Integration Layer
- `ImGuiBackend` abstraction for platform backends
- `ImGuiGlfwOpenGLBackend` concrete implementation for GLFW/OpenGL
- `ImGuiOverlay` singleton manager with DockSpace support
- `ImGuiWidget` base interface for custom widgets
- Semi-transparent window backgrounds (configurable)

#### ImGui Widgets
- `FpsWidget` for frame rate display and target FPS control
- `GamepadWidget` for real-time gamepad state visualization (#114)
  - Integrated settings panel for deadzone and axis inversion
  - Real-time stick position and button state display
  - Connection status indicator
  - Multi-controller support (1-4)
- `LogWidget` for scrollable log console with advanced filtering
  - Per-scope message buffers (1000 entries each)
  - Filter by log level and scope via ComboBox
  - Text search filter
  - Auto-scroll with pause on manual scroll
  - Scope-specific buffering on scope switch
- `CameraWidget` for camera parameter control
  - Position, LookAt target, and Up vector editing
  - Local/World space toggle for transforms
  - FOV, Near/Far plane, and Aspect Ratio controls
  - Quick view presets (Front, Top, Side, Isometric)
  - Reset to initial values
- `MainMenuWidget` for application settings
  - Window transparency control (slider + presets)
  - Docking toggle
  - Style themes (Dark, Light, Classic)
  - Settings persistence via `imgui.ini`

#### Logging System
- `LogSink` abstract interface with self-registering type identifiers
- `ConsoleSink` for stdout output
- `ImGuiLogSink` for LogWidget integration
- `LogManager` with enable/disable sinks by type identifier
- Scope-based filtering support

### Changed

#### Camera Architecture
- **BREAKING**: Cameras are now managed via `CameraSceneNode` instead of standalone `Camera` objects
- `Viewport` now holds `CameraSceneNode*` instead of `Camera*`
- View matrix computed from inverse of `CameraSceneNode::worldTransform()`
- Projection matrix remains responsibility of `Camera` (optics)

#### Logging System
- Refactored to use self-registering sinks
- Sinks define their own `TYPE_ID` string instead of central `SinkFlags` enum
- `LogManager::enableSink("console")` / `disableSink("imgui")` API
- Extensible without modifying core logging code

#### Core & Rendering
- **BREAKING**: Enum counter entries renamed to `size_` (#34)
- **BREAKING**: `MeshData` merged with `Mesh` (#22)
- `UniformValueMap::float_val` return type refactored (#33)
- `Material` ownership structure improved (#13)

### Fixed
- Potential nullptr dereference in `MaterialData` (#16)

### Documentation
- Added `CONVENTIONS.md` with LHS coordinate system documentation
- Matrix storage format (column-major) documentation
- View matrix construction explanation
- Units system documentation (`helios.core.units`)
- Updated API documentation for new modules
- Example code comments improved with section headers

## [Milestone 1] - 2025-10-21

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
  - Default loggers for specific scopes (e.g., `helios.rendering.shader`)
  - LogManager for centralized logger access

### Infrastructure
- CMake build system with C++23 modules support
- Cross-platform support (Windows, Linux, macOS)
- Unit testing framework with Google Test
- Example applications (simple cube rendering, game controller input)

---

## Release Notes

### Milestone 2 Breaking Changes

The following breaking changes are included in this release:

#### Camera System Refactor
Cameras are now integrated into the scene graph via `CameraSceneNode`:

```cpp
// Before (Milestone 1)
auto camera = std::make_shared<Camera>();
viewport->setCamera(camera.get());
camera->setPosition({0, 0, 5});
camera->lookAt({0, 0, 0}, {0, 1, 0});

// After (Milestone 2)
auto camera = std::make_unique<Camera>();
auto cameraNode = std::make_unique<CameraSceneNode>(std::move(camera));
auto* nodePtr = scene->addNode(std::move(cameraNode));
viewport->setCameraSceneNode(nodePtr);
nodePtr->setTranslation({0, 0, 5});
nodePtr->lookAt({0, 0, 0}, {0, 1, 0});
```

**Migration:** Replace `Camera` usage with `CameraSceneNode`. Use scene graph methods (`translate`, `rotate`) instead of direct camera manipulation.

#### Transform Inheritance
Child nodes can now selectively inherit transform components using `helios::math::TransformType`:

```cpp
import helios.math.transform;

// Camera follows parent position only, maintains own orientation
cameraNode->setInheritance(helios::math::TransformType::Translation);

// Full inheritance (default)
childNode->setInheritance(helios::math::TransformType::All);

// Combine flags
node->setInheritance(helios::math::TransformType::Translation | helios::math::TransformType::Rotation);
```

#### Enum Sentinel Naming (#34)
All enum counter/size entries have been renamed to `size_`:

```cpp
// Before
enum class Key { A, B, C, COUNT };

// After
enum class Key { A, B, C, size_ };
```

#### MeshData Removal (#22)
`MeshData` has been merged into `Mesh`:

```cpp
// Before
MeshData meshData = ...;
Mesh mesh(meshData);

// After
Mesh mesh = ...;
```

#### Material Ownership (#13)
Materials now own their shader and material data:

```cpp
// Before
MaterialData materialData(shader, properties);
Material material(materialData);

// After
auto shader = std::make_shared<Shader>(...);
auto properties = std::make_shared<MaterialProperties>(...);
auto material = std::make_shared<Material>(shader, properties);
```


---

## Links

- [Documentation](https://thorstensuckow.github.io/helios/)
- [GitHub Repository](https://github.com/thorstensuckow/helios)
- [Issue Tracker](https://github.com/thorstensuckow/helios/issues)
- [Contributing Guide](docs/CONTRIBUTING.md)

