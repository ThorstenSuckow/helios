# helios::engine::modules::effects::gfx

Visual effects components and systems.

## Overview

This module provides components and systems for visual effects such as rotation animations. Effects are purely visual and do not affect game logic.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpinComponent` | Stores angular velocity and rotation axis for spinning |
| `SpinSystem` | Applies spin rotation to entities each frame |

## Usage

```cpp
gameObject_.add<SpinComponent>();
gameObject_.get<SpinComponent>()->setSpeed(270.0f);
gameObject_.get<SpinComponent>()->setAxis(helios::math::Z_AXISf);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::effects::gfx
@brief Visual effects components and systems.
@details Provides purely visual effects like spinning/rotation that do not affect game logic.
</p></details>
