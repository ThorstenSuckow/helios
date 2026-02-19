# helios::engine::state::components

Components for storing state on entities.

## Components

| Class | Description |
|-------|-------------|
| `StateComponent<StateType>` | Stores current state and last transition |

## Usage

Typically attached to the session entity:

```cpp
auto& stateComponent = session.getOrAdd<StateComponent<GameState>>();
GameState current = stateComponent.state();
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state::components
@brief State storage components.
@details Provides component types for storing state information on entities.
</p></details>

