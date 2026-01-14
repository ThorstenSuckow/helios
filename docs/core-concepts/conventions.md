# helios Technical Conventions

This document describes the fundamental technical conventions used throughout the helios engine, including coordinate systems, matrix storage, and related mathematical foundations.

## Coordinate System

### Left-Handed System (LHS)

helios uses a **Left-Handed Coordinate System** (LHS) as its primary spatial convention:

- **+X axis**: Points to the right
- **+Y axis**: Points upward
- **+Z axis**: Points forward (into the screen)

This convention aligns with common game engine practices and provides intuitive "depth" semantics where positive Z values represent objects further away from the viewer.

### OpenGL Compatibility

While helios internally uses LHS, OpenGL expects a Right-Handed System (RHS) where the camera looks down the **-Z axis**. The engine handles this conversion transparently:

1. The `CameraSceneNode::worldTransform()` computes the View Matrix by inverting the camera's world transform
2. The Z-axis is negated during View Matrix construction to convert from LHS to OpenGL's RHS clip space
3. Near and far plane values remain positive regardless of the camera's orientation

**Example:** An object at world position `(0, 0, 5)` in helios (5 units in front of the origin) will be correctly rendered when the camera is positioned to view it, with the projection handling the LHS-to-RHS conversion.

## Matrix Conventions

### Storage Format: Column-Major Order

All `mat4` matrices in helios are stored in **column-major order**. This means:

- The internal array `m[16]` stores elements column by column
- `m[0..3]` = Column 0 (elements at rows 0-3)
- `m[4..7]` = Column 1 (elements at rows 0-3)
- `m[8..11]` = Column 2 (elements at rows 0-3)
- `m[12..15]` = Column 3 (elements at rows 0-3)

### Element Access

The `operator()(row, col)` provides intuitive mathematical notation:

```cpp
// Access element at row r, column c
T value = matrix(r, c);  // Internally: m[r + c * 4]
```

**Memory layout visualization:**

```
Mathematical notation:     Memory layout (column-major):
| m(0,0) m(0,1) m(0,2) m(0,3) |     m[0]  m[4]  m[8]  m[12]
| m(1,0) m(1,1) m(1,2) m(1,3) |     m[1]  m[5]  m[9]  m[13]
| m(2,0) m(2,1) m(2,2) m(2,3) |     m[2]  m[6]  m[10] m[14]
| m(3,0) m(3,1) m(3,2) m(3,3) |     m[3]  m[7]  m[11] m[15]
```

### Translation Components

In a standard 4x4 transformation matrix, the **translation vector** is stored in **column 3**:

```cpp
// Extracting translation from a transform matrix
helios::math::vec3f translation(
    matrix(0, 3),  // X translation (m[12])
    matrix(1, 3),  // Y translation (m[13])
    matrix(2, 3)   // Z translation (m[14])
);
```

### Rotation and Scale Components

The upper-left 3x3 submatrix contains rotation and scale:

```cpp
// Extracting basis vectors (columns)
vec3f xAxis(matrix(0, 0), matrix(1, 0), matrix(2, 0));  // Column 0
vec3f yAxis(matrix(0, 1), matrix(1, 1), matrix(2, 1));  // Column 1
vec3f zAxis(matrix(0, 2), matrix(1, 2), matrix(2, 2));  // Column 2
```

### Matrix Multiplication Order

helios uses **post-multiplication** (column-vector convention):

```cpp
// Transform a point: result = M * v
vec4f transformed = matrix * vec4f(point, 1.0f);

// Combining transforms: first A, then B
mat4f combined = B * A;  // A is applied first, then B
```

## View Matrix Construction

The View Matrix is computed as the inverse of the camera's World Transform. For a camera node:

```cpp
// In CameraSceneNode::worldTransform()
const auto wt = SceneNode::worldTransform();

// Extract basis vectors and eye position
const auto x = vec3f{wt(0, 0), wt(1, 0), wt(2, 0)};
const auto y = vec3f{wt(0, 1), wt(1, 1), wt(2, 1)};
const auto z = vec3f{wt(0, 2), wt(1, 2), wt(2, 2)};
const auto eye = vec3f{wt(0, 3), wt(1, 3), wt(2, 3)};

// Construct inverse (View Matrix) with Z-negation for OpenGL
mat4f viewMatrix = mat4f{
    x[0],  y[0], -z[0], 0.0f,
    x[1],  y[1], -z[1], 0.0f,
    x[2],  y[2], -z[2], 0.0f,
    -dot(x, eye), -dot(y, eye), dot(z, eye), 1.0f
};
```

The negation of the Z-axis converts from helios LHS to OpenGL's RHS expectation.

## Projection Matrix

Perspective projection uses standard parameters:

| Parameter | Description |
|-----------|-------------|
| `fovY` | Vertical field of view in **radians** |
| `aspectRatio` | Width / Height ratio |
| `zNear` | Near clipping plane distance (positive) |
| `zFar` | Far clipping plane distance (positive) |

Both `zNear` and `zFar` are specified as positive values representing distances from the camera, regardless of the coordinate system handedness.

## GameObject Active State

### The `isActive()` Flag

Every `GameObject` has an `isActive()` flag that determines whether it participates in the game loop:

```cpp
bool isActive() const noexcept;
void setActive(bool active) noexcept;
```

### Behavior

When a GameObject is **inactive**:

- It **exists** in the GameWorld and retains all its components and state
- It is **not rendered** (SceneNode is deactivated)
- It is **not processed** by systems (movement, physics, collision detection)
- It does **not participate** in gameplay calculations

This is essential for object pooling where pre-allocated objects wait in an inactive state until acquired.

### System Filtering

By default, systems should only process active GameObjects. Use `GameObjectFilter` to filter queries:

```cpp
import helios.engine.ecs.query.GameObjectFilter;

using helios::engine::ecs::query::GameObjectFilter;

void update(UpdateContext& ctx) noexcept override {
    // Filter for active GameObjects with enabled components
    auto filter = GameObjectFilter::Active | GameObjectFilter::ComponentEnabled;
    
    for (auto [obj, component] : gameWorld_->find<MyComponent>(filter).each()) {
        // Only active objects with enabled components
    }
}
```

Alternatively, filter manually within the loop:

```cpp
void update(UpdateContext& ctx) noexcept override {
    for (auto [obj, component] : gameWorld_->find<MyComponent>().each()) {
        if (!obj->isActive()) {
            continue;  // Skip inactive objects
        }
        if (component.isDisabled()) {
            continue;  // Skip disabled components
        }
        // Process active object with enabled component...
    }
}
```

### Lifecycle Callbacks

Components receive lifecycle notifications when their owning GameObject changes state:

```cpp
class MyComponent : public Component {
    void onActivate() noexcept override {
        // Called when GameObject becomes active
    }
    
    void onDeactivate() noexcept override {
        // Called when GameObject becomes inactive
    }
    
    void onAcquire() noexcept override {
        // Called when acquired from a pool
    }
    
    void onRelease() noexcept override {
        // Called when released back to a pool
    }
};
```

### Pool Integration

The active state integrates with the object pool system:

1. **Pool Creation:** Objects are created inactive
2. **Acquire:** Object is activated and `onAcquire()` is called on components
3. **Gameplay:** Object participates in all systems while active
4. **Release:** Object is deactivated and `onRelease()` is called, then returned to pool

## Units System

### Standard Units

helios uses a standardized unit system for consistent measurements across the engine:

| Measurement | Standard Unit | Description |
|-------------|---------------|-------------|
| Distance    | **Meter**     | 1 helios unit (hu) = 1 meter |
| Time        | **Seconds**   | Used for delta time, animations |

### Helios Units (hu)

The **helios unit** (hu) is the engine's standard unit of measurement:

- **1 hu = 1 Meter**
- All distances, positions, and sizes are expressed in meters by default

This provides intuitive world-scale dimensions that match real-world measurements.

### Usage Example

```cpp
import helios.core.units.Unit;

using namespace helios::core::units;

// Available unit types
enum class Unit { Meter, Centimeter, HeliosUnit };

// Set object size in world units (meters)
gameObject->setSize(2.0f, 1.0f, 0.5f, Unit::Meter);

// A 5-meter tall object
constexpr float HEIGHT = 5.0f; // 5 hu = 5 meters
```

### Constants

Unit definitions are available as `constexpr` in `helios::core::units`:

```cpp
// 1 helios unit = 1 meter
constexpr float HELIOS_UNITS_PER_METER = 1.0f;
```

## Related Modules

- `helios.core.units.Unit` — Unit conversion and constants
- `helios.math.types` — Core vector and matrix types (`vec3f`, `mat4f`)
- `helios.math.utils` — Mathematical utility functions (`perspective`, `radians`, `degrees`)
- `helios.scene.Transform` — Encapsulates translation, rotation, and scale
- `helios.scene.Camera` — Projection matrix management
- `helios.scene.CameraSceneNode` — View matrix computation and scene graph integration
- `helios.scene.InheritTransform` — Selective transform inheritance flags
