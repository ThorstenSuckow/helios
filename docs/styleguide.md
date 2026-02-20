# helios Code Style Guide

This document summarizes the coding style and module organization conventions used in the `helios` project. It is intended to ensure consistency and maintainability across the codebase.

### Quick summary
- Use C++23 modules (.ixx for interface/export, .cpp for implementation units) where appropriate.
- Filenames should reflect module and type names (PascalCase for modules/types, e.g. `Window.ixx` / `Window.cpp`).
- Namespace hierarchies mirror module paths (e.g. `helios::window`).
- Getters should be `[[nodiscard]]`; methods that do not throw should be `noexcept`.
- Member variables end with `_` (e.g. `width_`).


### Modules / file formats

### File organization
- **One public class per file.** Each exported class or struct gets its own `.ixx` file named after the type (PascalCase).
- **Exception - cyclic done()-chaining builders:** When fluent builder types form a cyclic dependency chain (e.g. `Rule::done() → Profile&`, `Profile::done() → Pool&`, `Pool::commit() → Configurator`, `Configurator::pool() → Pool`), they may reside in a single file because C++ modules do not support forward declarations across module boundaries. Document the reason in the file header.
- Private or implementation-only helper types (e.g. internal iterators, CRTP bases used only within one file) may co-locate with their owning class.

### Module interface files
- Interface files (exported modules) live under `include/helios/...` and use the `.ixx` extension (e.g. `include/helios/window/Window.ixx`).
  - Structure of an interface file:
    - Global module fragment (optional) with `module;` and any required `#include` directives for standard/third-party headers.
    - `export module helios.<...>.<Name>;`
    - `import` statements for dependent modules.
    - `export`ed types/namespaces the module exposes.
    - Doxygen comments for the public API.
- Implementation/module units typically live under `src/helios/...` as `.cpp` files and are module implementation units:
  - Start with `module;` (global fragment if needed), followed by `module helios.<...>.<Name>;` (without `export`).
  - `import` statements for required modules.
- Filename convention: PascalCase for module/type files (e.g. `Window.ixx`, `Window.cpp`).
- Module names reflect the path: `helios.window.Window` corresponds to `include/helios/window/Window.ixx`.

### Module facades and partitions
- Module facades are prefixed with an underscore `_` (e.g. `_module.ixx` or `_window` facades). They collect or re-export partitions of a subsystem.
- A module facade **MUST NOT** export lower-level modules - modules should only export their own partitions. (Existing rule kept.)

### Imports / includes / ordering
- Prefer `import` for internal project modules. Use `#include` only when a module is not available (e.g. external C headers).
- Recommended order in a file:
  1. Global module fragment: `module;` + `#include` directives (if needed)
  2. Module declaration: `export module ...;` (interface) or `module ...;` (implementation)
  3. `import` statements
  4. Macros / `#define` constants (only if unavoidable)
  5. `namespace` block with code

Example (interface):
```cpp 
module;
#include <string>
export module helios.window.Window;
import helios.util.Guid;
export namespace helios::window { ... }
```

Example (implementation):
```cpp 
module;
#include <string>
module helios.window.Window;
import helios.util.Guid;
namespace helios::window { ... }
```

### Namespaces
- Use the nested short notation `helios::sub::...` (as used throughout the code), not flat or global namespaces.
- Namespaces should reflect module paths.

### Class and member conventions
- Type names: PascalCase (e.g. `Window`, `Guid`, `SceneNode`).
- Method and variable names: lowerCamelCase (e.g. `shouldClose()`, `swapBuffers()`).
- Private/protected members end with `_` (e.g. `width_`, `guid_`).
- Recommended access order inside a class:
  1. private
  2. protected
  3. public
- Constructors with a single parameter should be `explicit`.
- Use `override` for overridden virtual methods.
- Virtual destructors in base classes: `virtual ~ClassName() = default;`.
- Getters that return a value or reference **should** be `[[nodiscard]]`.
- Functions that do not throw should be marked `noexcept`.

### Syntax and formatting
This project follows a consistent C++ syntax and formatting style to improve readability and reduce churn in reviews. Follow these concrete rules:

- Braces: opening brace on the same line as the declaration, for statements and definitions:
  - `if (condition) {` not
    ```cpp
    if (condition)
    {
    }
    ```
- Indentation: 4 spaces per indent level. Do not use tabs.
- Spaces: Put a single space after keywords and before parentheses where appropriate: `if (`, `for (`, `while (`. Use a single space around binary operators: `a + b`.
- Line length: Prefer <= 100 characters. Break long expressions across multiple indented lines (see initializer list example).
- Blank lines: Separate logical sections (e.g. between method definitions, between groups of members) with a single blank line.
- Trailing whitespace: Avoid any trailing whitespace on lines.
- Header/include order in module fragments: `module;`, then `#include`s, then `export module` / `module` declaration, then `import`s.
- Comments: Prefer concise `//` comments for short notes, and `/** ... */` Doxygen comments for public API elements.
- **No inline comments:** Do not place comments on the same line as code. Always place comments on a separate line above the element they describe.

Code examples (use these as canonical examples):

Module interface (public API example)
```cpp
// include/helios/window/Window.ixx
module;
#include <string>

export module helios.window.Window;

import helios.util.Guid;

export namespace helios::window {

class Window {
public:
    explicit Window(const WindowConfig& cfg);

    [[nodiscard]] const util::Guid& guid() const noexcept;

    virtual bool show() noexcept = 0;
    virtual void swapBuffers() const noexcept = 0;

    virtual ~Window() = default;
};

} // namespace helios::window
```

Implementation (definition / initializer list example)
```cpp
// src/helios/window/Window.cpp
module;
#include <string>

module helios.window.Window;

import helios.util.Guid;

namespace helios::window {

    Window::Window(const WindowConfig& cfg)
        : width_(cfg.width),
          height_(cfg.height),
          title_(cfg.title),
          viewport_(cfg.viewport),
          guid_(util::Guid::generate()) {
    }
    
    [[nodiscard]] const util::Guid& Window::guid() const noexcept {
        return guid_;
    }

} // namespace helios::window
```

Brace and spacing examples
```cpp
// preferred
if (shouldClose()) {
    logger_.info("Window requested close");
}

// not preferred
if (shouldClose())
{
    logger_.info("Window requested close");
}
```

Override example
```cpp
class GlfwWindow : public Window {
public:
    bool show() noexcept override;
    void swapBuffers() const noexcept override;
};
```

Logging macro usage
```cpp
#define HELIOS_LOG_SCOPE "helios::window::Window"
const helios::util::log::Logger& logger_ =
    helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);
```

Splitter / comment for namespace end
```cpp
} // namespace helios::window
```

### Attributes and qualifiers
- Use `[[nodiscard]]` for all value- or status-returning getters.
- Use `noexcept` for methods that are guaranteed not to throw (e.g. `guid() const noexcept`).
- [Const-correctness](https://isocpp.org/wiki/faq/const-correctness) is mandatory for methods that do not modify object state.

### Logging
- Pattern used in the project:
  - Define a macro scope at the top of the interface file (`#define HELIOS_LOG_SCOPE "helios::window::Window"`).
  - Logger instance as a protected member:
    const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);
- Use `LogManager::getInstance()` centrally; the scope should always contain the fully qualified module/type package.

### Documentation
- Public API must include Doxygen comments (in the interface `.ixx`).
- Provide a short description, parameters, return values, and notes about side effects or threading when applicable.

### README Files
Each module directory should contain a `README.md` file following this structure:

1. **Title:** Use the C++ namespace as the heading (with `::` separators), prefixed with `#`:
   ```markdown
   # helios::engine::modules::physics
   ```
   Not the module name (`helios.engine.modules.physics`).

2. **Description:** A brief paragraph describing the module's purpose.

3. **Doxygen Section:** At the end, include a collapsible Doxygen block:
   ```markdown
   ---
   <details>
   <summary>Doxygen</summary><p>
   @namespace helios::engine::modules::physics
   @brief Physics simulation and collision detection.
   @details Extended description...
   </p></details>
   ```

4. **Consistency:** The namespace in the title and `@namespace` tag must match exactly.

Example README:
```markdown
# helios::window

Window management and platform abstraction layer.

This module provides the abstract Window interface and related event handling...

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::window
@brief Window management and platform abstraction.
@details Provides the Window base class and event types for window lifecycle...
</p></details>
```

### Tests, examples, benchmarks
- Tests live under `tests/helios/...`, benchmarks under `benchmarks/helios/...`, and examples under `examples/`.
- Tests should import modules the same way production code does and follow the same naming conventions.

### Do's and Don'ts (project-specific)
- Do: Use modules (`.ixx`) for public-facing APIs.
- Do: Export only the actual API from an interface file.
- Do: Keep the member naming suffix `_` and use `[[nodiscard]]` / `noexcept` where appropriate.
- Don't: Use global namespaces or unqualified logger scopes.
