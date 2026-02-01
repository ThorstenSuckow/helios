# helios::rendering::text::config

This namespace provides configuration classes for text rendering shader properties, including base properties and per-instance overrides.

| Class | Description |
|-------|-------------|
| `TextShaderProperties` | Immutable base properties (e.g., text color) shared via `TextRenderPrototype` |
| `TextShaderPropertiesOverride` | Optional per-instance overrides for individual `TextRenderable` objects |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::text::config
@brief Configuration classes for text shader properties.
@details This namespace provides configuration classes for text rendering shader properties. `TextShaderProperties` defines immutable base properties shared across multiple text renderables, while `TextShaderPropertiesOverride` allows per-instance customization.
</p></details>
