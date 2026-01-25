# helios::engine::modules::effects::gfx::systems

Visual effects systems.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpinSystem` | Applies rotation from SpinComponent each frame |

## Usage

```cpp
// Add SpinSystem to game loop (Main phase)
gameLoop.phase(PhaseType::Main).addPass()
    .addSystem<SpinSystem>(&gameWorld);

// SpinSystem reads SpinComponent and writes to RotationStateComponent
// The rotation is then composed by ComposeTransformSystem
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::effects::gfx::systems
@brief Visual effects systems.
@details Contains systems that process visual effect components.
</p></details>
