# helios::ui::layout::components

Layout-related UI components.

This namespace contains components used by the UI layout system for formatting and presenting data in text widgets.

## Components

| Component | Description |
|-----------|-------------|
| `TimeFormatterComponent` | Formats elapsed or remaining time into a display string via `std::vformat` |
| `NumberFormatterComponent` | Formats arithmetic values (scores, health, etc.) into a display string via `std::vformat` |

## TimeFormatterComponent

Holds a format string and a `TimeDisplayMode`. The format string receives two integer arguments (minutes, seconds). When duration is zero, the display mode falls back to `Elapsed`.

## NumberFormatterComponent

Holds a `std::vformat`-compatible format string that accepts a single arithmetic argument. Used by binding systems such as `Score2UiTextUpdateSystem` and `MaxScore2UiTextUpdateSystem` to convert numeric game state into text.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::ui::layout::components
@brief Layout-related UI components.
@details Contains components for time and number formatting used in conjunction with binding systems such as GameTimer2UiTextUpdateSystem, Score2UiTextUpdateSystem, and MaxScore2UiTextUpdateSystem.
</p></details>

