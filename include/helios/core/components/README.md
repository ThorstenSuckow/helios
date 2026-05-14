# helios::core::components

Lightweight value components for shared math types.

`helios.core.components` provides small, reusable ECS components for common
math primitives, scalar values, and a consistent dirty-state workflow.

## Components

| Component | Stored Type |
|-----------|-------------|
| `Vec2Component<TDomainTag, THandle, TNumericType>` | `helios::math::vec2<TNumericType>` |
| `Vec3Component<TDomainTag, THandle, TNumericType>` | `helios::math::vec3<TNumericType>` |
| `Vec4Component<TDomainTag, THandle, TNumericType>` | `helios::math::vec4<TNumericType>` |
| `Mat4Component<TDomainTag, THandle, TNumericType>` | `helios::math::mat4<TNumericType>` |
| `AABBComponent<TDomainTag, THandle, TNumericType>` | `helios::math::aabb<TNumericType>` |
| `NumericValueComponent<TDomainTag, THandle, TNumericType>` | `TNumericType` |
| `ColorComponent<THandle>` | `helios::math::vec4<float>` |

## Aliases

- `ColorComponent<THandle>` is a convenience alias for RGBA-style color values
  backed by `Vec4Component<ColorDomainTag, THandle, float>`.
- `NumericValueComponent<TDomainTag, THandle, TNumericType>` is the scalar
  counterpart to the vector and matrix components and is useful for values such
  as intensities, IDs, thresholds, or OpenGL object identifiers.

## Behavior

All components in this module follow the same pattern:

- start dirty
- `setValue(...)` sets dirty only when value changes
- `onAcquire()` and `onRelease()` set dirty
- `clearDirty()` resets dirty after processing

`NumericValueComponent` uses exact comparison for integral types and an epsilon-
based comparison for floating-point types to avoid unnecessary dirty updates for
small scalar deltas.

## Minimal Usage

```cpp
import helios.core.components;

struct WorldTag;
using Handle = MyEntity::Handle_type;

auto& v = entity.add<helios::core::components::Vec3Component<WorldTag, Handle, float>>({0.0f, 1.0f, 0.0f});
if (v.isDirty()) {
    v.clearDirty();
}

auto& color = entity.add<helios::core::components::ColorComponent<Handle>>({1.0f, 0.5f, 0.25f, 1.0f});
auto& exposure = entity.add<helios::core::components::NumericValueComponent<WorldTag, Handle, float>>(1.0f);
```


