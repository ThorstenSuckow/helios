# helios::engine::common::tags

Compile-time tag types for engine role classification.

## Overview

Tag types are empty structs used as compile-time markers. A class declares
its engine role by adding a nested type alias:

```cpp
using EngineRoleTag = helios::engine::common::tags::SystemTag;
```

The [concepts](../concepts/README.md) module detects this alias via `HasTag`
and uses it to gate registration APIs, preventing accidental registration of
types that happen to have matching method signatures.

## Tags

| Tag | Role | Detected by |
|-----|------|-------------|
| `ManagerTag` | Batch-processing manager | `IsManager<T>` |
| `SystemTag` | Per-frame game logic processor | `IsSystem<T>` |

## Usage

```cpp
class MySystem {
public:
    using EngineRoleTag = helios::engine::common::tags::SystemTag;

    void update(UpdateContext& ctx) noexcept {
        // per-frame logic
    }
};
```

## See Also

- [Concepts](../concepts/README.md) — Role concepts consuming these tags
- [System](../../../../docs/core-concepts/ecs/system.md) — System architecture
- [Command System](../../../../docs/core-concepts/command-system.md) — Manager registration

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::common::tags
@brief Compile-time tag types for engine role classification.
</p></details>

