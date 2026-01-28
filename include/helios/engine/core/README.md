# helios::engine::core

Core engine infrastructure providing fundamental building blocks for the helios engine.

This module contains essential infrastructure components that are used throughout the helios engine. It provides the foundational systems upon which higher-level game and rendering logic is built.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `data/` | Typed IDs for pools and spawn profiles |
| `units/` | Canonical length and time units (meters, seconds) |

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObjectPoolId` | Strongly-typed identifier for object pools |
| `SpawnProfileId` | Strongly-typed identifier for spawn profiles |
| `SpawnRuleId` | Strongly-typed identifier for spawn rules |
| `Unit` | Enum for length/time units (Meter, Centimeter, Seconds) |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::core
@brief Core engine infrastructure providing fundamental building blocks.
@details This module contains essential infrastructure components used throughout the helios engine, including data structures for entity management, messaging systems for commands and events, and unit definitions for consistent measurements.
</p></details>


