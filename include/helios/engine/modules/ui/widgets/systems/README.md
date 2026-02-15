# helios::engine::modules::ui::widgets::systems

UI widget processing systems.

This namespace contains systems that process UI widget components, handling tasks such as navigation, styling, and bounds updates.

## Systems

| System | Description |
|--------|-------------|
| `MenuNavigationSystem` | Handles gamepad input for menu navigation |
| `UiStyleUpdateSystem` | Applies visual styles based on interaction state |
| `UiTextBoundsUpdateSystem` | Updates AABB bounds when text content changes |

## MenuNavigationSystem

Processes gamepad Up/Down input to change menu selection and A button to activate items. Updates `UiStateComponent` selection state and issues `UiActionCommand` on activation.

## UiStyleUpdateSystem

Queries entities with `UiTextComponent`, `UiStyleComponent`, and `UiStateComponent`. Applies appropriate colors and scales based on the current interaction state (normal, selected, disabled).

## UiTextBoundsUpdateSystem

Queries entities with `UiTextComponent` and `ModelAabbComponent`. When the text component signals `needsResize()`, the system retrieves the new AABB from the underlying `TextRenderable` and updates `ModelAabbComponent`.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::widgets::systems
@brief UI widget processing systems.
@details Contains systems for processing UI widget state changes including menu navigation, style application, and AABB bounds synchronization with text content.
</p></details>
