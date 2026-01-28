# helios::engine::mechanics::input

Input handling systems for game entities.

## Overview

This module provides input-related functionality for the game layer, including systems that translate device input (gamepads, keyboards) into game commands.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `systems/` | Input processing systems (TwinStickInputSystem) |

## Key Classes

| Class | Purpose |
|-------|---------|
| `TwinStickInputSystem` | Reads gamepad stick input and enqueues Move2D/Aim2D commands |

## Usage

```cpp
// Create player with required components
auto player = GameObjectFactory::gameObject()
    .withMotion([](auto& m) {
        m.move2D().speed(5.0f).acceleration(10.0f);
        m.steering().instantSteering(false);
    })
    .make(true);

// Create input system for gamepad control
auto inputSystem = std::make_unique<TwinStickInputSystem>(
    &gameWorld,
    &inputManager,
    helios::input::types::Gamepad::ONE,
    player->guid()  // Target entity for input commands
);

// Register in Pre phase (before movement systems)
gameLoop.phase(PhaseType::Pre).addPass()
    .addSystem(std::move(inputSystem));
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::input
@brief Input handling systems for game entities.
@details This module provides input-related functionality for the game layer, including systems that translate device input into game commands.
</p></details>