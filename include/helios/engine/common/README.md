# helios::engine::common

Shared type definitions, compile-time concepts, and role tags used across multiple engine subsystems.

## Overview

This module provides lightweight, system-agnostic infrastructure shared by
all engine subsystems: interaction context structs, compile-time concepts
for role classification, and tag types for the opt-in registration pattern.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `concepts/` | C++20 concepts for engine role constraints (IsManager, IsSystem, ...) |
| `tags/` | Empty tag structs for compile-time role identification (ManagerTag, SystemTag) |
| `types/` | Context structs for entity interactions and damage |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::common
@brief Shared type definitions, concepts, and tags used across engine subsystems.
</p></details>

