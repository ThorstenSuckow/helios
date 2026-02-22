# helios::engine::modules::ui::layout

Layout primitives for UI positioning and time formatting.

This namespace provides fundamental layout types used by the UI system to position elements relative to their parent viewport or container, as well as components for formatting time values.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Layout-related components (e.g. time formatting) |
| `types/` | Layout type definitions (anchors, display modes) |

## Types

| Type | Description |
|------|-------------|
| `Anchor` | Enum defining anchor points for UI element positioning |
| `TimeDisplayMode` | Enum controlling elapsed vs. remaining time display |

## Components

| Component | Description |
|-----------|-------------|
| `TimeFormatterComponent` | Formats elapsed or remaining time into a display string via `std::vformat` |

## Anchor Values

| Value | Description |
|-------|-------------|
| `Center` | Element is anchored to the center of the viewport |
| `TopRight` | Element is anchored to the top-right corner |
| `TopLeft` | Element is anchored to the top-left corner |
| `BottomLeft` | Element is anchored to the bottom-left corner |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::layout
@brief Layout primitives for UI positioning and time formatting.
@details This namespace contains fundamental types for UI layout, including anchor points that define how UI elements are positioned relative to their parent viewport, as well as time formatting components and display mode types.
</p></details>
