# helios::engine::modules::ui::widgets::systems

UI widget processing systems.

This namespace contains systems that process UI widget components, handling tasks such as bounds updates when content changes.

## Systems

| System | Description |
|--------|-------------|
| `UiTextBoundsUpdateSystem` | Updates AABB bounds when text content changes |

## UiTextBoundsUpdateSystem

Queries entities with `UiTextComponent` and `ModelAabbComponent`. When the text component signals `needsResize()`, the system retrieves the new AABB from the underlying `TextRenderable` and updates `ModelAabbComponent`.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::widgets::systems
@brief UI widget processing systems.
@details Contains systems for processing UI widget state changes. UiTextBoundsUpdateSystem ensures AABB bounds stay synchronized with text content.
</p></details>
