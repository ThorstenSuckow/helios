# helios::engine::mechanics::gamestate::listeners

Listeners for reacting to game state transitions.

## Listeners

| Listener | Description |
|----------|-------------|
| `FocusMenuListener` | Focuses a menu when entering the Paused state |
| `WorldResetListener` | Resets the game world on specific transitions |

## FocusMenuListener

Listens for transitions to the `Paused` state and sets focus to a configured menu. Clears focus when exiting the `Paused` state.

## WorldResetListener

Triggers a world reset on transitions such as:
- Title → Running (game start)
- Paused → Title (quit to menu)
- Paused → Running (restart)
- Running → Title (exit)

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate::listeners
@brief Listeners for reacting to game state transitions.
@details Contains listener implementations that respond to state changes.
</p></details>

