# helios::engine::mechanics::gamestate::systems

Systems for game state management.

## Systems

| System | Description |
|--------|-------------|
| `GameStateInputResponseSystem` | Handles input for state transitions |

## GameStateInputResponseSystem

Listens for the gamepad Start button and issues appropriate `GameStateCommand` based on the current state:

- **Title** + Start → `StartRequested` transition
- **Running** + Start → `TogglePause` transition

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate::systems
@brief Systems for game state management.
@details Contains ECS systems that handle game state logic.
</p></details>

