# helios::engine::modules::ui::binding::systems

UI data binding systems.

This namespace contains systems that connect game state to UI widgets, enabling automatic UI updates when the underlying data changes.

## Systems

| System | Description |
|--------|-------------|
| `Score2UiTextUpdateSystem` | Updates text components when observed score values change |

## Score2UiTextUpdateSystem

Queries entities with both `ScoreObserverComponent` and `UiTextComponent`. When the score observer signals an update, the system propagates the new total score to the text component.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::binding::systems
@brief UI data binding systems.
@details Contains systems that observe game state and update UI widgets accordingly. Score2UiTextUpdateSystem provides score-to-text binding.
</p></details>
