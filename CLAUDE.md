# CLAUDE.md - AI Assistant Guide for helios

This document provides comprehensive guidance for AI assistants working with the **helios** codebase. It covers repository structure, development workflows, coding conventions, and best practices specific to this project.

## Table of Contents

- [Project Overview](#project-overview)
- [Repository Structure](#repository-structure)
- [Technology Stack](#technology-stack)
- [Development Workflow](#development-workflow)
- [Code Style and Conventions](#code-style-and-conventions)
- [Module System](#module-system)
- [Building and Testing](#building-and-testing)
- [Common Tasks](#common-tasks)
- [Key Architectural Patterns](#key-architectural-patterns)
- [Important Files](#important-files)
- [Git Workflow](#git-workflow)
- [Documentation Standards](#documentation-standards)

---

## Project Overview

**helios** is a custom game framework built from first principles in modern C++23. It is an educational project exploring game engine architecture, emphasizing:

- **Modern C++23**: Module-based architecture with clean dependency management
- **First Principles**: Building mathematical and architectural foundations from scratch
- **Educational Focus**: Understanding both theory and practical implementation
- **Performance-Critical Domain**: Applying modern software engineering in real-time graphics

### Purpose
The framework provides core systems for game development:
- Scene graph with hierarchical transforms
- Rendering pipeline with customizable passes
- Input system (keyboard, mouse, gamepad)
- Math library for 3D graphics
- OpenGL 4.6+ rendering backend
- Cross-platform support (Windows, Linux, macOS)

### Academic Context
This originated as a computer science semester project. The design and implementation are documented in:
- **[Suc25]** Suckow-Homberg, Thorsten: *helios: Design and prototypical implementation of a C++ game framework* (2025)

---

## Repository Structure

```
helios/
├── benchmarks/         # Performance benchmarks for classes/modules
├── docs/              # Documentation (style guides, API docs, tutorials)
│   ├── CONTRIBUTING.md         # Contribution guidelines
│   ├── styleguide.md          # C++ coding style guide
│   ├── doxygen-style.md       # Documentation standards
│   ├── heliosapi.md           # API reference overview
│   └── issue_submission_guide.md
├── examples/          # Example applications and tutorials
│   ├── simple_cube_rendering/
│   └── game_controller_input/
├── include/           # Module interface units (.ixx)
│   ├── helios/        # Core framework modules
│   │   ├── app/           # Application lifecycle
│   │   ├── input/         # Input handling
│   │   ├── math/          # Vectors, matrices, transforms
│   │   ├── rendering/     # Rendering pipeline
│   │   ├── scene/         # Scene graph
│   │   ├── util/          # Logging, utilities
│   │   └── window/        # Window management
│   └── ext/           # Platform-specific extensions (GLFW, OpenGL)
│       ├── glfw/
│       └── opengl/
├── src/               # Module implementation units (.cpp)
│   ├── helios/        # Core implementations
│   └── ext/           # Extension implementations
├── tests/             # Unit tests (Google Test)
│   ├── helios/
│   └── ext/
├── website/           # Project website
├── .clang-format      # Code formatting rules
├── CMakeLists.txt     # Root build configuration
├── CHANGELOG.md       # Version history
└── README.md          # Project readme
```

### Key Directories

#### `include/helios/`
Module interface units (`.ixx`) for the core framework:
- **Public API**: All exported types and functions
- **Module declarations**: `export module helios.<subsystem>.<Name>;`
- **Doxygen comments**: Required for all public APIs

#### `src/helios/`
Module implementation units (`.cpp`) corresponding to interfaces in `include/helios/`:
- **Implementation details**: Not exported from modules
- **Module implementation units**: `module helios.<subsystem>.<Name>;` (no `export`)

#### `include/ext/` & `src/ext/`
Platform-specific implementations using vendor libraries:
- **GLFW**: Window management, input
- **OpenGL**: Rendering backend using GLAD

#### `tests/`
Unit tests using Google Test framework:
- Mirrors `src/` structure
- Test naming: `TEST(ComponentTest, DescribesBehavior)`

---

## Technology Stack

### Core Technologies
- **Language**: C++23 with modules
- **Build System**: CMake 4.0+
- **Graphics API**: OpenGL 4.6+ (via GLAD v2)
- **Window/Input**: GLFW 3.4
- **Testing**: Google Test
- **Documentation**: Doxygen

### Compiler Requirements
- **MSVC**: 19.38+ (Visual Studio 2022)
- **GCC**: 13.2+
- **Clang**: 17+

### Dependencies
Managed via CMake FetchContent:
- **GLFW** 3.4 (window/input)
- **GLAD** v2.0.8 (OpenGL loader)

---

## Development Workflow

### Prerequisites

1. **Required Tools**:
   - C++23 compatible compiler
   - CMake 4.0+
   - Git

2. **IDE Setup**:
   - Visual Studio 2022+ (Windows)
   - CLion, VS Code (cross-platform)
   - Ensure C++23 module support is enabled

### Setting Up Development Environment

```bash
# Clone the repository
git clone https://github.com/thorstensuckow/helios.git
cd helios

# Configure with CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run tests
cd build
ctest -C Release --output-on-failure
```

### Branch Strategy

- **main**: Stable releases and milestones
- **feature/**: Feature branches (e.g., `feature/particle-system`)
- **fix/**: Bug fix branches
- **refactor/**: Refactoring work

---

## Code Style and Conventions

### Critical Style Rules

#### File Naming
- **Module interfaces**: PascalCase with `.ixx` extension (e.g., `Window.ixx`, `SceneNode.ixx`)
- **Implementations**: PascalCase with `.cpp` extension (e.g., `Window.cpp`, `SceneNode.cpp`)
- **Module facades**: Prefixed with `_` (e.g., `_module.ixx`, `_window.ixx`)

#### Naming Conventions
- **Types/Classes**: PascalCase (`Window`, `RenderQueue`, `MaterialProperties`)
- **Methods/Variables**: lowerCamelCase (`shouldClose()`, `swapBuffers()`, `baseColor`)
- **Member Variables**: Trailing underscore (`width_`, `height_`, `guid_`)
- **Namespaces**: lowercase, mirror module paths (`helios::window`, `helios::rendering`)

#### Class Structure
```cpp
export namespace helios::window {

class Window {
private:
    int width_;
    int height_;
    std::string title_;

protected:
    util::Guid guid_;
    const util::log::Logger& logger_;

public:
    explicit Window(const WindowConfig& cfg);

    [[nodiscard]] const util::Guid& guid() const noexcept;
    [[nodiscard]] bool shouldClose() const noexcept;

    virtual void swapBuffers() const noexcept = 0;
    virtual ~Window() = default;
};

} // namespace helios::window
```

**Access Order**: `private` → `protected` → `public`

#### Key Attributes
- **`[[nodiscard]]`**: ALL getters that return values (mandatory)
- **`noexcept`**: Methods guaranteed not to throw
- **`explicit`**: Single-parameter constructors
- **`override`**: Overridden virtual methods
- **`const`**: Methods not modifying state (const-correctness is mandatory)

#### Formatting (enforced by `.clang-format`)
- **Indentation**: 4 spaces (no tabs)
- **Braces**: Opening brace on same line
  ```cpp
  if (condition) {  // ✓ Correct
      // code
  }

  if (condition)    // ✗ Wrong
  {
      // code
  }
  ```
- **Line Length**: ≤ 120 characters (prefer ≤ 100)
- **Pointer Alignment**: Left (`int* ptr` not `int *ptr`)
- **Namespace Indentation**: All levels indented

#### Constructor Initialization
```cpp
Window::Window(const WindowConfig& cfg)
    : width_(cfg.width),
      height_(cfg.height),
      title_(cfg.title),
      viewport_(cfg.viewport),
      guid_(util::Guid::generate()) {
}
```
**Pattern**: Colon on same line as closing paren, one initializer per line, aligned

### Before Committing Code

**Always run clang-format**:
```bash
clang-format -i path/to/changed/files.cpp
clang-format -i path/to/changed/files.ixx
```

---

## Module System

### Module Structure

helios uses C++23 modules extensively. Understanding the module system is critical.

#### Module Interface Unit (`.ixx`)
Located in `include/helios/`:

```cpp
// include/helios/window/Window.ixx
module;                           // Global module fragment
#include <string>                 // Standard headers go here
#include <memory>

export module helios.window.Window;  // Module declaration

import helios.util.Guid;          // Import dependencies

export namespace helios::window { // Export namespace

/**
 * @brief Represents a platform window.
 *
 * Detailed Doxygen documentation here...
 */
class Window {
    // ... class definition
};

} // namespace helios::window
```

#### Module Implementation Unit (`.cpp`)
Located in `src/helios/`:

```cpp
// src/helios/window/Window.cpp
module;                           // Global module fragment
#include <string>

module helios.window.Window;      // NO 'export' keyword

import helios.util.Guid;

namespace helios::window {        // NO 'export' keyword

Window::Window(const WindowConfig& cfg)
    : width_(cfg.width),
      height_(cfg.height) {
}

} // namespace helios::window
```

### Module Naming Convention

**Pattern**: `helios.<subsystem>.<Type>`

Examples:
- `helios.window.Window` → `include/helios/window/Window.ixx`
- `helios.rendering.RenderQueue` → `include/helios/rendering/RenderQueue.ixx`
- `helios.math.types.vec3` → `include/helios/math/types/vec3.ixx`

### Module Facades

Module facades (prefixed with `_`) aggregate partitions or provide convenience imports:

```cpp
// include/helios/input/_module.ixx
export module helios.input;

export import helios.input.InputManager;
export import helios.input.InputAdapter;
export import helios.input.GamepadState;
export import helios.input.types;
```

**Rule**: Facades MUST NOT export lower-level modules, only their own partitions.

### Import/Include Order

1. **Global module fragment**: `module;`
2. **Includes**: `#include` directives (standard library, third-party)
3. **Module declaration**: `export module` or `module`
4. **Imports**: `import` statements
5. **Code**: Namespace and definitions

---

## Building and Testing

### Build Configuration

#### Debug Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

#### Release Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

#### Windows (Visual Studio)
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### Build Options

Controlled via CMakeLists.txt options:

```bash
-DBUILD_TESTING=ON       # Build tests (default: ON)
-DBUILD_BENCHMARKS=ON    # Build benchmarks (default: ON)
-DBUILD_EXAMPLES=ON      # Build examples (default: ON)
```

### Running Tests

```bash
# All tests
cd build
ctest -C Debug --output-on-failure

# Specific test
ctest -C Debug -R RenderQueueTest --output-on-failure

# Verbose output
ctest -C Debug -V
```

### Test Structure

Tests use Google Test framework:

```cpp
#include <gtest/gtest.h>
import helios.rendering.RenderQueue;

TEST(RenderQueueTest, AddsRenderCommandSuccessfully) {
    auto queue = RenderQueue();
    auto command = std::make_unique<RenderCommand>(...);

    queue.add(std::move(command));

    EXPECT_EQ(queue.count(), 1);
}

TEST(RenderQueueTest, HandlesNullRenderCommandGracefully) {
    auto queue = RenderQueue();

    EXPECT_NO_THROW(queue.add(nullptr));
    EXPECT_EQ(queue.count(), 0);
}
```

**Test Naming**: `TEST(ComponentTest, DescribesBehavior)`
- Test both success and failure cases
- Use descriptive names
- Place in `tests/helios/<module>/`

---

## Common Tasks

### Adding a New Feature

1. **Plan the changes**:
   - Identify affected modules
   - Check for breaking changes
   - Review architectural fit

2. **Create interface (`.ixx`)**:
   ```cpp
   // include/helios/subsystem/NewFeature.ixx
   module;
   #include <...>

   export module helios.subsystem.NewFeature;

   import ...;

   export namespace helios::subsystem {
       class NewFeature {
           // Public API with Doxygen comments
       };
   }
   ```

3. **Create implementation (`.cpp`)**:
   ```cpp
   // src/helios/subsystem/NewFeature.cpp
   module;
   #include <...>

   module helios.subsystem.NewFeature;

   import ...;

   namespace helios::subsystem {
       // Implementation
   }
   ```

4. **Add to CMakeLists.txt**:
   ```cmake
   # In appropriate CMakeLists.txt
   target_sources(helios_subsystem
       PUBLIC
           FILE_SET CXX_MODULES
           FILES
               ${INCLUDE_DIR}/helios/subsystem/NewFeature.ixx
       PRIVATE
           ${SRC_DIR}/helios/subsystem/NewFeature.cpp
   )
   ```

5. **Write tests**:
   ```cpp
   // tests/helios/subsystem/NewFeatureTest.cpp
   #include <gtest/gtest.h>
   import helios.subsystem.NewFeature;

   TEST(NewFeatureTest, DescribesBehavior) {
       // Test implementation
   }
   ```

6. **Update documentation**:
   - Add Doxygen comments to public API
   - Update relevant docs in `docs/`
   - Add example if appropriate

7. **Format and test**:
   ```bash
   clang-format -i include/helios/subsystem/NewFeature.ixx
   clang-format -i src/helios/subsystem/NewFeature.cpp
   cmake --build build --config Debug
   ctest -C Debug --output-on-failure
   ```

### Fixing a Bug

1. **Write a failing test** that reproduces the bug
2. **Fix the bug** in the implementation
3. **Verify the test passes**
4. **Check for regressions** with full test suite
5. **Update documentation** if behavior changed

### Refactoring Code

1. **Ensure tests exist** for the code being refactored
2. **Make incremental changes**
3. **Run tests after each change**
4. **Update documentation** for API changes
5. **Mark breaking changes** with `!` in commit message

---

## Key Architectural Patterns

### Rendering Pipeline Flow

```
Scene → Snapshot → RenderPass → RenderQueue → RenderCommand → GPU
```

1. **Scene**: Contains SceneNodes with transforms and renderables
2. **Snapshot**: Immutable view of scene at a point in time
3. **RenderPass**: Frame-specific uniforms and settings
4. **RenderQueue**: Collection of RenderCommands for a pass
5. **RenderCommand**: Low-level GPU command (draw call)

### Prototype-Instance Pattern

Used for efficient rendering with shared data:

```cpp
// Prototype: Shared immutable data (Mesh + Material)
auto prototype = std::make_shared<RenderPrototype>(material, mesh);

// Renderable: Instance with optional per-instance overrides
auto renderable = std::make_shared<Renderable>(prototype);
renderable->materialOverride() = MaterialPropertiesOverride{
    .baseColor = vec4f(1.0f, 0.0f, 0.0f, 1.0f)
};
```

**Key Types**:
- **`RenderPrototype`**: Immutable `Mesh` + `Material`
- **`Renderable`**: Instance of prototype with optional `MaterialPropertiesOverride`
- **`Material`**: `Shader` + default `MaterialProperties`
- **`MaterialPropertiesOverride`**: Per-instance property overrides

### Scene Graph Hierarchy

```cpp
auto scene = std::make_unique<Scene>(cullingStrategy);

// Add camera
auto camera = std::make_unique<Camera>();
scene->addNode(std::move(camera));

// Add nodes with parent-child relationships
auto parentNode = std::make_unique<SceneNode>(renderable);
auto* parent = scene->addNode(std::move(parentNode));

auto childNode = std::make_unique<SceneNode>(childRenderable);
parent->addChild(std::move(childNode));

// Transforms propagate from parent to child
parent->translate(vec3f(1.0f, 0.0f, 0.0f));
parent->rotate(rotationMatrix);
```

### Logging Pattern

All classes use centralized logging:

```cpp
// In interface (.ixx), global module fragment:
#define HELIOS_LOG_SCOPE "helios.subsystem.ClassName"

// In class definition:
protected:
    const helios::util::log::Logger& logger_ =
        helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);

// Usage:
logger_.info("Message");
logger_.warn("Warning");
logger_.error("Error");
```

**Scope format**: Always fully qualified module path (e.g., `"helios.rendering.RenderQueue"`)

---

## Important Files

### Configuration Files

#### `.clang-format`
Enforces code style. Run before every commit:
```bash
clang-format -i <files>
```

#### `CMakeLists.txt`
Root build configuration:
- C++23 standard requirement
- FetchContent for dependencies (GLFW, GLAD)
- Subdirectories for src, tests, benchmarks, examples

### Documentation Files

#### `docs/CONTRIBUTING.md`
Contribution guidelines covering:
- Commit conventions (Conventional Commits)
- Pull request process
- Code review expectations

#### `docs/styleguide.md`
Comprehensive C++ style guide:
- Module structure
- Naming conventions
- Formatting rules
- Code examples

#### `docs/doxygen-style.md`
Documentation standards for Doxygen comments

#### `docs/heliosapi.md`
High-level API overview with usage examples

#### `docs/issue_submission_guide.md`
Guidelines for bug reports, feature requests, refactoring proposals

### Changelog

#### `CHANGELOG.md`
Version history following Keep a Changelog format:
- Unreleased changes
- Breaking changes with migration guides
- Milestone releases

---

## Git Workflow

### Commit Conventions

helios follows [Conventional Commits](https://www.conventionalcommits.org/) strictly.

#### Format
```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

#### Types

- **`feat:`** - New feature
- **`fix:`** - Bug fix
- **`refactor:`** - Code change (no bug fix, no feature)
- **`docs:`** - Documentation only
- **`style:`** - Formatting (no code change)
- **`test:`** - Adding or fixing tests
- **`perf:`** - Performance improvement
- **`build:`** - Build system or dependencies
- **`ci:`** - CI configuration
- **`chore:`** - Other changes (no src/test modification)

#### Scopes

Common scopes:
- `rendering` - Rendering system
- `scene` - Scene management
- `input` - Input handling
- `math` - Math library
- `window` - Window management
- `opengl` - OpenGL backend
- `glfw` - GLFW integration

#### Examples

**Simple feature**:
```
feat(input): add mouse wheel support
```

**Bug fix with details**:
```
fix(window): prevent crash on window resize during rendering

The window could be resized while a frame was being rendered,
causing a race condition. This adds proper synchronization.

Fixes #156
```

**Breaking change**:
```
refactor(math)!: change matrix storage to column-major

BREAKING CHANGE: Matrix indices are now column-major to match OpenGL.
Code accessing matrix elements directly must be updated.

Migration:
- Before: matrix[row][col]
- After:  matrix[col][row]

Closes #245
```

#### Description Rules

- **MUST** be lowercase (except proper nouns, acronyms, code refs)
- **MUST** use imperative mood ("add feature" not "added feature")
- **MUST NOT** end with period
- **SHOULD** be concise (≤ 50 characters)

**Good**:
```
fix: prevent race condition in scene graph updates
feat: add support for HDR rendering
```

**Bad**:
```
Fix: Prevented race condition in scene graph updates.
feat: Added support for HDR rendering
Fixed a bug
```

### Pull Request Process

1. **Update branch** with latest upstream:
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Run full test suite**:
   ```bash
   cmake --build build --config Debug
   cd build
   ctest -C Debug --output-on-failure
   ```

3. **Format code**:
   ```bash
   clang-format -i path/to/changed/files.*
   ```

4. **Build in both Debug and Release**:
   ```bash
   cmake --build build --config Debug
   cmake --build build --config Release
   ```

5. **Create PR** with:
   - Clear title following commit conventions
   - Description referencing issues (`Fixes #123`)
   - Explanation of what changed and why
   - List of breaking changes
   - Testing instructions

---

## Documentation Standards

### Doxygen Comments

**All public APIs MUST have Doxygen comments**. Follow `docs/doxygen-style.md`.

#### Example

```cpp
/**
 * @brief Adds a RenderCommand to the queue.
 *
 * Ownership of the RenderCommand is transferred to the queue.
 * If the command is nullptr, it will be silently ignored and
 * an error will be logged.
 *
 * @param renderCommand The command to add. May be nullptr.
 *
 * @warning This method is not thread-safe.
 * @see RenderCommand, RenderQueue
 */
void add(std::unique_ptr<const RenderCommand> renderCommand);
```

#### Required Elements

- **`@brief`**: Short description (one line)
- **`@param`**: For each parameter
- **`@return`**: If function returns a value
- **`@throw`**: If function may throw exceptions
- **`@warning`**: For thread-safety or other important notes
- **`@see`**: Related classes/methods

### Updating Documentation

When changes affect:
- **Public APIs**: Update Doxygen comments
- **Architecture**: Update relevant docs in `docs/`
- **Features**: Add examples to `examples/`
- **Breaking Changes**: Add migration guide to `CHANGELOG.md`

---

## Best Practices for AI Assistants

### When Reading Code

1. **Check module structure first**: Look at `.ixx` for public API, `.cpp` for implementation
2. **Follow imports**: Understand dependencies via `import` statements
3. **Read Doxygen comments**: They contain contract and usage information
4. **Check tests**: They show intended usage and edge cases

### When Writing Code

1. **Always start with the interface** (`.ixx`):
   - Define public API with Doxygen comments
   - Use proper attributes (`[[nodiscard]]`, `noexcept`)
   - Follow naming conventions exactly

2. **Match the project style**:
   - Use existing files as templates
   - Follow the style guide strictly
   - Run clang-format before committing

3. **Write tests first or alongside**:
   - Test both success and failure cases
   - Follow existing test patterns
   - Ensure tests pass before committing

4. **Update documentation**:
   - Add/update Doxygen comments
   - Update relevant docs in `docs/`
   - Add examples if introducing new features

### When Suggesting Changes

1. **Check for breaking changes**:
   - API modifications affecting existing code
   - Changes to public interfaces
   - Mark with `!` in commit message if breaking

2. **Consider architectural fit**:
   - Does it align with existing patterns?
   - Is it consistent with the educational goals?
   - Would it introduce unnecessary complexity?

3. **Verify cross-platform compatibility**:
   - Windows, Linux, macOS support required
   - Use standard C++23 features
   - Avoid platform-specific code in core modules

### Common Pitfalls to Avoid

1. **Don't skip Doxygen comments** on public APIs
2. **Don't forget `[[nodiscard]]`** on getters
3. **Don't mix tabs and spaces** (use 4 spaces)
4. **Don't use `using namespace`** in header/interface files
5. **Don't export lower-level modules** from facades
6. **Don't forget trailing underscore** on member variables
7. **Don't skip clang-format** before committing
8. **Don't use global namespaces** (always `helios::subsystem`)

---

## Quick Reference

### File Extensions
- `.ixx` - Module interface unit (in `include/`)
- `.cpp` - Module implementation unit (in `src/`)
- `.h` - C-style headers (rare, for third-party)

### Key Commands

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --config Debug

# Test
cd build && ctest -C Debug --output-on-failure

# Format
clang-format -i file.cpp file.ixx

# Run example
cd build/examples/simple_cube_rendering/Release
./simple_cube_rendering
```

### Module Declaration Templates

**Interface**:
```cpp
module;
#include <...>
export module helios.subsystem.Name;
import ...;
export namespace helios::subsystem { ... }
```

**Implementation**:
```cpp
module;
#include <...>
module helios.subsystem.Name;
import ...;
namespace helios::subsystem { ... }
```

---

## Additional Resources

- **[README.md](README.md)** - Project overview and quick start
- **[docs/CONTRIBUTING.md](docs/CONTRIBUTING.md)** - Detailed contribution guide
- **[docs/styleguide.md](docs/styleguide.md)** - Complete style guide
- **[docs/heliosapi.md](docs/heliosapi.md)** - API reference
- **[examples/](examples/)** - Working code examples
- **[CHANGELOG.md](CHANGELOG.md)** - Version history

---

## Contact & Support

- **GitHub Issues**: https://github.com/thorstensuckow/helios/issues
- **Discussions**: https://github.com/thorstensuckow/helios/discussions
- **Email**: thorsten@suckow-homberg.de

---

**Last Updated**: 2025-11-15
**helios Version**: Milestone 1+
**Document Version**: 1.0
