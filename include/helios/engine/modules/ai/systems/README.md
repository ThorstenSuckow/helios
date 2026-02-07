# helios::engine::modules::ai::systems

AI behavior processing systems.

## Overview

Systems that process AI components and update entity behavior. Registered with the GameLoop and executed during the appropriate phase.

## Systems

| System | Purpose |
|--------|---------|
| `ChaseSystem` | Steers entities towards their chase targets |

## ChaseSystem

Processes entities with `ChaseComponent`, `SteeringComponent`, and `TranslationStateComponent`.

### Behavior

1. **Target Resolution**: Looks up target entity by handle
2. **Timer Update**: Advances cooldown timer by delta time
3. **Cooldown Check**: Only updates direction when timer exceeds cooldown
4. **Direction Calculation**: Computes normalized vector from entity to target
5. **Steering Update**: Sets steering intent to face calculated direction

### Required Components

| Component | Purpose |
|-----------|---------|
| `ChaseComponent` | Target handle and timing configuration |
| `SteeringComponent` | Receives steering intent updates |
| `TranslationStateComponent` | Provides current entity position |

### Registration

```cpp
import helios.engine.modules.ai.systems;

// Register with GameLoop main phase
gameLoop.addSystem<ChaseSystem>(Phase::Main, gameWorld);
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.modules.ai.components` | ChaseComponent data |
| `helios.engine.modules.physics.motion` | SteeringComponent |
| `helios.engine.modules.spatial.transform` | TranslationStateComponent |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ai::systems
@brief AI behavior processing systems.
@details Systems that process AI components to update entity behavior. ChaseSystem steers entities towards their targets based on cooldown timing and direction calculations.
</p></details>
