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

The View Matrix is computed as the inverse of the camera's World Transform. Since `helios` uses an orthonormal basis for transforms (Rotation + Translation), the inversion is optimized by transposing the rotation matrix and computing the inverse translation.

Additionally, the Z-axis is negated to convert from the engine's Left-Handed System (LHS) to OpenGL's Right-Handed System (RHS).

```cpp
// In CameraSceneNode::onWorldTransformUpdate()
const auto wt = SceneNode::cachedWorldTransform();

// Extract basis vectors and position
const auto x = helios::math::vec3f{wt(0, 0), wt(1, 0), wt(2, 0)};
const auto y = helios::math::vec3f{wt(0, 1), wt(1, 1), wt(2, 1)};
const auto z = helios::math::vec3f{wt(0, 2), wt(1, 2), wt(2, 2)};
const auto eye = helios::math::vec3f{wt(0, 3), wt(1, 3), wt(2, 3)};

// Compute view matrix: inverse of world transform with Z-negation for OpenGL RHS
// Rotation is transposed (inverse of orthonormal matrix)
// Translation is: -dot(axis, eye)
auto viewMatrix = helios::math::mat4f{
    x[0],          y[0],          -z[0],        0.0f,
    x[1],          y[1],          -z[1],        0.0f,
    x[2],          y[2],          -z[2],        0.0f,
    -dot(x, eye), -dot(y, eye),    dot(z, eye), 1.0f
};
```

## Units of Measurement

helios enforces a strict unit convention to ensure consistency across physics, rendering, and game logic.

### Spatial Units

The standard unit of length in helios is the **Meter**.

- **1.0 Helios Unit = 1.0 Meter**

All spatial coordinates, distances, and sizes in the engine (e.g., `transform.position`, `mesh.vertices`) are expressed in meters.

#### Helper Constants

The `helios::core::units` module provides constants and conversion helpers:

- `helios::core::units::METERS` (1.0)
- `helios::core::units::CENTIMETERS` (0.01)

### Temporal Units

The standard unit of time in helios is the **Second**.

- **1.0 Helios Time Unit = 1.0 Second**

All durations, timestamps, and delta times (e.g., `deltaTime`, `animation.duration`) are expressed in seconds.

#### Helper Constants

- `helios::core::units::SECONDS` (1.0)
- `helios::core::units::MILLISECONDS` (0.001)

### Usage Example

```cpp
import helios.core.units;

using namespace helios::core::units;

// Define a distance of 50 centimeters
float distance = fromCm(50.0f); // Returns 0.5f (meters)

// Define a duration of 100 milliseconds
float duration = fromMs(100.0f); // Returns 0.1f (seconds)

// Generic conversion
float val = from(100.0f, Unit::Centimeter); // Returns 1.0f
```

## Transform Inheritance

Scene graph nodes can selectively inherit transform components from their parent using `helios::math::TransformType`. This enables flexible hierarchical behaviors without full transform coupling.

### TransformType Flags

The `TransformType` enum provides bitmask flags for selective inheritance:

| Flag | Description |
|------|-------------|
| `Translation` | Inherit only the parent's position offset |
| `Rotation` | Inherit only the parent's orientation |
| `Scale` | Inherit only the parent's scale factors |
| `All` | Inherit all components (default behavior) |

### Usage Example

```cpp
import helios.math.transform;

using namespace helios::math;

// Camera follows spaceship position but maintains independent orientation
cameraNode->setInheritance(TransformType::Translation);

// Combine multiple flags
node->setInheritance(TransformType::Translation | TransformType::Rotation);

// Full inheritance (default)
childNode->setInheritance(TransformType::All);
```

### Matrix Decomposition

The `mat4::decompose()` member function extracts specific transform components from a matrix:

```cpp
import helios.math.types;
import helios.math.transform;

mat4f fullTransform = /* scaled, rotated, translated */;

// Extract only translation
mat4f translationOnly = fullTransform.decompose(TransformType::Translation);

// Extract rotation without scale
mat4f rotationOnly = fullTransform.decompose(TransformType::Rotation);

// Transpose a matrix (useful for inverting orthonormal rotation matrices)
mat4f transposed = rotationMatrix.transpose();
```

## Related Modules

- `helios.core.units` — Unit conversion and constants
- `helios.math.types` — Core vector and matrix types (`vec3f`, `mat4f`) with `decompose()` and `transpose()`
- `helios.math.utils` — Mathematical utility functions (`perspective`, `radians`, `degrees`)
- `helios.math.transform` — Transform utilities including `TransformType`
- `helios.scene.Transform` — Encapsulates translation, rotation, and scale
- `helios.scene.Camera` — Projection matrix management
- `helios.scene.CameraSceneNode` — View matrix computation and scene graph integration


