# helios::gameplay::match::events

Events related to match rule processing.

## Events

| Event | Description |
|-------|-------------|
| `PlayerDiedEvent` | Emitted when the player entity dies |

## PlayerDiedEvent

Published when the player entity's health is depleted. Contains:

- `source()` – Handle of the deceased player entity

Consumed by match-rule systems to trigger life loss, game-over
transitions, or respawn logic.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::match::events
@brief Events related to match rule processing.
@details Contains event classes for player death and match-level state changes.
</p></details>

