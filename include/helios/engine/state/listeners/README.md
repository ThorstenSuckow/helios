# helios::engine::state::listeners

Listeners for observing state transitions.

## Components

| Class | Description |
|-------|-------------|
| `LambdaStateListener<StateType>` | Lambda-based listener implementation |

## Usage

```cpp
using namespace helios::engine::state::listeners;

auto listener = std::make_unique<LambdaStateListener<GameState>>(
    // onExit
    [](auto& ctx, GameState from) {
        // Handle state exit
    },
    // onTransition
    [](auto& ctx, auto transitionCtx) {
        // Handle transition
    },
    // onEnter
    [](auto& ctx, GameState to) {
        // Handle state entry
    }
);

stateManager.addStateListener(std::move(listener));
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state::listeners
@brief State transition listeners.
@details Provides listener implementations for reacting to state changes.
</p></details>

