# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- `FramePacer` class for frame rate control (#111)
- `FrameStats` structure for frame timing information (#111)
- `FpsMetrics` class for frame rate analysis (#111)
- `Stopwatch` high-resolution timer (#111)
- `FpsWidget` for ImGui overlays
- `GamepadWidget` for ImGui overlays (#114)
- ImGui integration layer
  - `ImGuiBackend` abstraction for platform backends
  - `ImGuiGlfwOpenGLBackend` concrete implementation
  - `ImGuiOverlay` singleton manager for widget rendering
  - `ImGuiWidget` base interface for custom widgets

### Fixed
- **BREAKING**: Enum counter entries renamed to `size_` (#34)
  - Consistent naming across all enums
  - Improved code generation and Doxygen compatibility
  - Uses trailing underscore to avoid macro conflicts
- **BREAKING**: `MeshData` merged with `Mesh` (#22)
  - Simplified architecture by removing redundant abstraction
  - Updated all references throughout codebase
  - Documentation updated to reflect changes
- `UniformValueMap::float_val` return type refactored (#33)
  - Changed from pointer to `std::optional` for safer API
  - Better matches intended usage patterns
- `Material` ownership structure (#13)
  - Material now owns both `Shader` and `MaterialData`
  - MaterialData is now optional (can be nullptr)
  - Reduced tight coupling and unnecessary indirection
  - Cleaner hierarchy: `Material → shared_ptr<Shader> + shared_ptr<MaterialData>`

### Fixed
- Potential nullptr dereference in `MaterialData` (#16)
  - Added proper null checks
  - Improved safety in material handling

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

### Upcoming Breaking Changes (Unreleased)

The following breaking changes are planned for the next release:

#### Enum Sentinel Naming (#34)
All enum counter/size entries have been renamed to `size_`:

```cpp
// Before
enum class Key {
    A, B, C,
    COUNT  // or SIZE, or size, etc.
};

// After
enum class Key {
    A, B, C,
    size_  // consistent across codebase
};
```

**Migration:** Search and replace enum size entries to use `size_` consistently.

#### MeshData Removal (#22)
`MeshData` has been merged into `Mesh`, simplifying the architecture:

```cpp
// Before
MeshData meshData = ...;
Mesh mesh(meshData);

// After
Mesh mesh = ...;
```

**Migration:** Remove `MeshData` references and use `Mesh` directly.

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

**Migration:** Update Material construction to pass shader and properties directly.

---

## Links

- [Documentation](https://thorstensuckow.github.io/helios/)
- [GitHub Repository](https://github.com/thorstensuckow/helios)
- [Issue Tracker](https://github.com/thorstensuckow/helios/issues)
- [Contributing Guide](docs/contributing/guide.md)

