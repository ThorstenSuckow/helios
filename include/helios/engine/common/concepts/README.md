# helios::engine::common::concepts

Compile-time concepts for engine role classification and capability detection.

## Overview

This module provides C++20 concepts that constrain types for specific engine
roles (System, Manager, CommandBuffer, CommandHandler). The concepts combine
structural requirements (e.g. `flush()`, `update()`) with explicit opt-in via
the [tag system](../tags/README.md), preventing accidental registration of
types that happen to have matching method signatures.

## Concepts

### Capability Detection

| Concept | Requires | Purpose |
|---------|----------|---------|
| `HasTag<T, Tag>` | `T::EngineRoleTag == Tag` | Detects compile-time role tag |
| `HasInit<T>` | `init(GameWorld&)` | Detects optional one-time initialization |
| `HasUpdate<T>` | `update(UpdateContext&)` | Detects per-frame update capability |
| `HasSubmit<T, Cmd>` | `submit(const Cmd&) noexcept -> bool` | Detects command submission capability |
| `HasClear<T>` | `clear() -> void` | Detects clear capability |
| `HasReset<T>` | `reset() -> void` | Detects optional reset capability |

### Role Constraints

| Concept | Requires | Purpose |
|---------|----------|---------|
| `IsManagerLike<T>` | `flush(UpdateContext&)` + `ManagerRole` | Gates `GameWorld::registerResource<T>()` |
| `IsSystemLike<T>` | `update(UpdateContext&)` + `SystemRole` | Gates `SystemRegistry::add<T>()` |
| `IsCommandBufferLike<T>` | `flush(GameWorld&, UpdateContext&)` + `HasClear` | Gates command buffer registration |
| `IsCommandHandlerLike<T, ...Cmds>` | `submit(const Cmd&) noexcept -> bool` for each `Cmd` | Gates command handler registration |

### Structural Constraints

| Concept | Requires | Purpose |
|---------|----------|---------|
| `IsShaderLike<T>` | `use()` + `applyUniformValues(UniformValueMap&)` | Constrains types usable as shader abstractions |

## Tag-Based Opt-In

Role concepts combine structural checks with a tag requirement:

```cpp
template<class T>
concept IsManagerLike = requires(T& t, UpdateContext& ctx) {
    { t.flush(ctx) } -> std::same_as<void>;
} && HasTag<T, ManagerRole>;
```

This means a type must both implement the required interface **and** declare
`using EngineRoleTag = ManagerRole;` to be eligible for registration.

## See Also

- [Tags](../tags/README.md) — Role tag types
- [System](../../../../docs/core-concepts/ecs/system.md) — System architecture
- [Command System](../../../../docs/core-concepts/command-system.md) — Manager and handler registration

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::common::concepts
@brief Compile-time concepts for engine role classification and capability detection.
</p></details>

