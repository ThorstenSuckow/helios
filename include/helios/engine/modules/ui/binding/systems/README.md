# helios::engine::modules::ui::binding::systems

UI data binding systems.

This namespace contains systems that connect game state to UI widgets, enabling automatic UI updates when the underlying data changes.

## Systems

| System | Description |
|--------|-------------|
| `Score2UiTextUpdateSystem` | Updates text components when observed score values change |
| `MaxScore2UiTextUpdateSystem` | Updates text components when observed high score values change |
| `GameTimer2UiTextUpdateSystem` | Updates text components when observed game timer values change |

## Score2UiTextUpdateSystem

Queries entities with both `ScoreObserverComponent` and `UiTextComponent`. When the score observer signals an update, the system propagates the new total score to the text component.

## MaxScore2UiTextUpdateSystem

Queries entities with both `MaxScoreObserverComponent` and `UiTextComponent`. When the max score observer signals an update, the system propagates the new high score to the text component.

## GameTimer2UiTextUpdateSystem

Queries entities with `GameTimerObserverComponent`, `TimeFormatterComponent`, and `UiTextComponent`. When the observed timer's revision changes, the system formats the current time via the `TimeFormatterComponent` and updates the text component.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::binding::systems
@brief UI data binding systems.
@details Contains systems that observe game state and update UI widgets accordingly. Provides score-to-text, high-score-to-text, and game-timer-to-text binding.
</p></details>
