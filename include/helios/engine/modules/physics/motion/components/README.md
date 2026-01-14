# helios::engine::modules::physics::motion::components

Physics and movement components for game entities.

This namespace contains components that handle physics simulation, movement, rotation, and related behaviors.

## Components

| Component | Description |
|-----------|-------------|
| `Move2DComponent` | 2D movement physics: velocity, acceleration, dampening. |
| `SteeringComponent` | Heading/rotation physics: turn speed, rotation dampening. |
| `DirectionComponent` | Stores a normalized direction vector. |
| `SpinComponent` | Continuous rotation around an axis (visual effects). |
| `RotationStateComponent` | Composite rotation state from heading and spin. |

## Usage

`Move2DComponent` and `SteeringComponent` work together for ship-like movement:

```cpp
auto& move = gameObject.add<Move2DComponent>();
move.setMaxSpeed(30.0f);
move.setAcceleration(20.0f);

auto& steering = gameObject.add<SteeringComponent>();
steering.setRotationSpeed(360.0f);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::physics::motion::components
@brief Physics and movement components for game entities.
@details This namespace contains components that handle 2D physics simulation, movement velocity, rotation/heading, and spin effects.
</p></details>
