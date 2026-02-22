# helios::engine::modules::ui::layout::components

Layout-related UI components.

This namespace contains components used by the UI layout system for formatting and presenting time-based data.

## Components

| Component | Description |
|-----------|-------------|
| `TimeFormatterComponent` | Formats elapsed or remaining time into a display string via `std::vformat` |

## TimeFormatterComponent

Holds a format string and a `TimeDisplayMode`. The format string receives two integer arguments (minutes, seconds). When duration is zero, the display mode falls back to `Elapsed`.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::layout::components
@brief Layout-related UI components.
@details Contains components for time formatting used in conjunction with binding systems such as GameTimer2UiTextUpdateSystem.
</p></details>

