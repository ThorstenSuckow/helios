# helios::state::commands

Commands for requesting state transitions.

## Components

| Class | Description |
|-------|-------------|
| `StateCommand<StateType>` | Encapsulates a transition request |

## Usage

```cpp
using namespace helios::state::commands;
using namespace helios::state::types;

// Request a state transition
updateContext.queueCommand<EngineCommandBuffer, StateCommand<GameState>>(
    StateTransitionRequest<GameState>{
        GameState::Running,
        GameStateTransitionId::Pause
    }
);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::state::commands
@brief State transition commands.
@details Provides command types for requesting state transitions through the command buffer.
</p></details>

