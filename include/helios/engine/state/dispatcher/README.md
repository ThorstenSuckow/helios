# helios::engine::state::dispatcher

Dispatchers for routing state commands.

## Components

| Class | Description |
|-------|-------------|
| `StateCommandDispatcher<StateType>` | Routes StateCommand to registered handler |

## Usage

Dispatchers are typically registered with the command buffer:

```cpp
commandBuffer.registerDispatcher<StateCommandDispatcher<GameState>>();
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state::dispatcher
@brief State command dispatchers.
@details Provides dispatcher implementations for routing state commands to their handlers.
</p></details>

