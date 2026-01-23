# helios::engine::modules::ai

AI behavior modules for game entities in the helios engine.

## Overview

This module provides components and systems for artificial intelligence behaviors. Currently focused on simple pursuit/chase mechanics, with architecture designed for expansion to more complex AI patterns.

## Submodules

### components/

Data components storing AI behavior state:

| Component | Purpose |
|-----------|---------|
| `ChaseComponent` | Tracks a target entity and controls pursuit timing |

### systems/

Systems that process AI components:

| System | Purpose |
|--------|---------|
| `ChaseSystem` | Steers entities towards their chase targets |

## Usage

### Basic Chase Behavior

```cpp
import helios.engine.modules.ai;

// Create enemy that chases player
auto enemy = std::make_unique<GameObject>();
enemy->add<ChaseComponent>();
enemy->get<ChaseComponent>()->setTarget(player->guid());
enemy->get<ChaseComponent>()->setCooldown(0.5f);  // Update direction every 0.5s

// Required companion components
enemy->add<SteeringComponent>();
enemy->add<TranslationStateComponent>();
```

### Using with GameObjectFactory

```cpp
auto enemy = GameObjectFactory::gameObject()
    .withAi([&](auto& ai) {
        ai.chasing()
          .target(player->guid())
          .seekInterval(0.5f);
    })
    .withMotion([](auto& m) {
        m.steering().instantSteering(true);
        m.move2D().speed(3.0f);
    })
    .make(true);
```

## Chase Behavior Details

The `ChaseSystem` operates as follows:

1. **Target Lookup**: Finds target entity by GUID in GameWorld
2. **Cooldown Check**: Only updates direction after cooldown expires
3. **Direction Calculation**: Computes normalized vector from entity to target
4. **Steering Update**: Sets steering intent to face target direction

```
┌─────────────────────────────────────────────────────────────┐
│                      ChaseSystem                            │
│  ┌─────────────────────────────────────────────────────────┐│
│  │ For each entity with:                                   ││
│  │   - ChaseComponent                                      ││
│  │   - SteeringComponent                                   ││
│  │   - TranslationStateComponent                           ││
│  ├─────────────────────────────────────────────────────────┤│
│  │ 1. Find target by GUID                                  ││
│  │ 2. Update cooldown timer                                ││
│  │ 3. If timer >= cooldown:                                ││
│  │    - Calculate direction to target                      ││
│  │    - Set steering intent                                ││
│  │    - Reset timer                                        ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## Component Dependencies

| Component | Required Companions |
|-----------|---------------------|
| `ChaseComponent` | `SteeringComponent`, `TranslationStateComponent` |

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.modules.physics.motion` | SteeringComponent for direction control |
| `helios.engine.modules.spatial.transform` | TranslationStateComponent for position |
| `helios.engine.builder` | ChaseConfig for fluent setup |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ai
@brief AI behavior modules for game entities.
@details This module provides components and systems for artificial intelligence behaviors, including chase/pursuit mechanics. ChaseComponent stores target and timing data, while ChaseSystem processes entities to steer them towards their targets.
</p></details>
