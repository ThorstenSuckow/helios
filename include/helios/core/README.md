# helios::core

Core-level utilities shared across the helios engine.

This module provides foundational infrastructure including double-buffering for thread-safe message passing, spatial transformations, type definitions, and data structures used by higher-level subsystems.

## Modules

### types

The `helios.core.types` module provides core type definitions and tag types used throughout the engine.

#### Tag Types

| Type | Description |
|------|-------------|
| `no_init_t` | Tag type to skip default initialization |
| `no_init` | Convenience constant for uninitialized construction |

#### Uninitialized Construction

The `no_init` tag enables performance-critical code to skip default initialization when objects will be immediately overwritten:

```cpp
import helios.util.Guid;
import helios.core.types;

// Generate a new unique Guid
auto id = helios::util::Guid::generate();

// Declare uninitialized Guid for later assignment
helios::util::Guid deferredId{helios::core::types::no_init};

// Assign later when the value is known
deferredId = helios::util::Guid::generate();
```

> **Warning:** Objects constructed with `no_init` are in an indeterminate state. Reading from them before assignment is undefined behavior.

#### Supporting no_init in Custom Types

The `helios::util::Guid` class demonstrates how to add `no_init` support:

```cpp
import helios.core.types;

class Guid final {
    uint64_t value_{};

public:
    // Skip initialization for deferred assignment
    explicit Guid(helios::core::types::no_init_t) {}

    // Factory method for generating valid Guids
    static Guid generate() noexcept;
};
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core
@brief Core utilities shared across the helios engine.
@details This module provides foundational infrastructure including double-buffering, spatial transformations, type definitions, and data structures used by higher-level subsystems.
</p></details>



