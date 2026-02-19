# helios::engine::state::commands

Commands for requesting state transitions.

## Components

| Class | Description |
|-------|-------------|
| `StateCommand<StateType>` | Encapsulates a transition request |

## Usage

```cpp
using namespace helios::engine::state::commands;
using namespace helios::engine::state::types;

// Request a state transition
updateContext.commandBuffer().add<StateCommand<GameState>>(
    StateTransitionRequest<GameState>{
        GameState::Running,
        GameStateTransitionId::Pause
    }
);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::state::commands
@brief State transition commands.
@details Provides command types for requesting state transitions through the command buffer.
</p></details>

