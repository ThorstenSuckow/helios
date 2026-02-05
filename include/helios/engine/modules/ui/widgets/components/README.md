# helios::engine::modules::ui::widgets::components

UI widget state components.

This namespace contains components that store the state and content of UI widgets.

## Components

| Component | Description |
|-----------|-------------|
| `UiTextComponent` | Text display with template-based formatting |

## UiTextComponent

Wraps a `TextRenderable` and provides template-based value display. Supports dirty tracking to minimize updates.

### Key Methods

| Method | Description |
|--------|-------------|
| `setTemplate()` | Sets the format template (uses `std::format` syntax) |
| `setDouble()` | Sets the numeric value to display |
| `needsResize()` | Returns `true` if bounds need recalculation |
| `resizeComplete()` | Marks resize as complete |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::widgets::components
@brief UI widget state components.
@details Contains components for UI widgets. UiTextComponent provides template-based text display with dirty tracking for efficient updates.
</p></details>
