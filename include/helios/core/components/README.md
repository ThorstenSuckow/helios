# helios::core::components

Lightweight value components for shared math types.

`helios.core.components` provides small, reusable ECS components for common
math primitives and a consistent dirty-state workflow.

## Components

| Component | Stored Type |
|-----------|-------------|
| `Vec2Component<TDomainTag, THandle, TNumericType>` | `helios::math::vec2<TNumericType>` |
| `Vec3Component<TDomainTag, THandle, TNumericType>` | `helios::math::vec3<TNumericType>` |
| `Mat4Component<TDomainTag, THandle, TNumericType>` | `helios::math::mat4<TNumericType>` |
| `AABBComponent<TDomainTag, THandle, TNumericType>` | `helios::math::aabb<TNumericType>` |

## Behavior

All components in this module follow the same pattern:

- start dirty
- `setValue(...)` sets dirty only when value changes
- `onAcquire()` and `onRelease()` set dirty
- `clearDirty()` resets dirty after processing

## Minimal Usage

```cpp
import helios.core.components;

struct WorldTag;
using Handle = MyEntity::Handle_type;

auto& v = entity.add<helios::core::components::Vec3Component<WorldTag, Handle, float>>({0.0f, 1.0f, 0.0f});
if (v.isDirty()) {
    v.clearDirty();
}
```


