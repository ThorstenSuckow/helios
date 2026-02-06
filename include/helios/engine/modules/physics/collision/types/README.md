# helios::engine::modules::physics::collision::types

Type definitions for collision system behavior configuration.

## Overview

This module provides enumeration types and configuration structures used by the collision detection system to control how collisions are processed and resolved.

## Key Types

| Type | Purpose |
|------|---------|
| `CollisionBehavior` | Bitmask enum defining collision response types |
| `CollisionContext` | Struct containing collision event data |

## CollisionBehavior

Bitmask values that can be combined to define collision responses:

| Value | Description |
|-------|-------------|
| `None` | No collision response |
| `Reflect` | Reflect velocity off the collision surface |
| `Bounce` | Bounce with restitution applied |
| `Stick` | Attach entity to the collision surface |
| `Despawn` | Remove entity from the game world |
| `PassEvent` | Emit event without physical response |
| `PushEvent` | Push event to double-buffered event bus |
| `FrameEvent` | Process event in the current frame |

## CollisionContext

Contains collision event data:

| Field | Type | Description |
|-------|------|-------------|
| `source` | `EntityHandle` | Source entity handle |
| `contact` | `vec3f` | World-space contact point |
| `isSolid` | `bool` | True if blocking collision |
| `isTrigger` | `bool` | True if trigger collision |
| `isCollisionReporter` | `bool` | True if source reports events |
| `other` | `optional<EntityHandle>` | Other entity handle (if any) |
| `collisionLayerId` | `uint32_t` | Source entity layer |
| `otherCollisionLayerId` | `uint32_t` | Other entity layer |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::physics::collision::types
@brief Type definitions for collision system behavior configuration.
@details Provides enums and structs that configure collision detection and response behavior, including bitmask-based behavior flags and collision event context data.
</p></details>

