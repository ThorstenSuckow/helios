
# Scene Graph

The scene graph is a hierarchical data structure that organizes all objects in your 3D world. It provides automatic transform propagation from parent to child nodes.

## Overview

helios uses a **tree-based scene graph** where each node can have:
- A local transformation (position, rotation, scale)
- A world transformation (computed from parent hierarchy)
- An optional renderable (mesh + material)
- Child nodes

## Key Classes

### `Scene`

The root container for all scene objects.

```cpp
import helios.scene.Scene;
import helios.scene.CullNoneStrategy;

auto scene = std::make_unique<Scene>(
    std::make_unique<CullNoneStrategy>()
);
```

### `SceneNode`

A node in the scene graph hierarchy.

```cpp
import helios.scene.SceneNode;

// Create a node with a renderable
auto node = std::make_unique<SceneNode>(std::move(renderable));

// Add as child to scene
auto* nodePtr = scene->addNode(std::move(node));

// Transform the node
nodePtr->translate(vec3f(0.0f, 2.0f, 0.0f));
nodePtr->rotate(rotationMatrix);
nodePtr->scale(vec3f(0.5f, 0.5f, 0.5f));
```

### `Camera`

Defines the viewpoint and projection.

```cpp
import helios.scene.Camera;

auto camera = std::make_unique<Camera>();
scene->addNode(std::move(camera));
```

## Transform Propagation

Transformations automatically propagate from parent to child:

```
Root (identity)
  ↓
Parent (translate(0, 2, 0))
  ↓
Child (rotate(45°))
  → Final transform: translate + rotate
```

**Example:**

```cpp
// Create parent-child hierarchy
auto parentNode = std::make_unique<SceneNode>();
auto* parent = scene->addNode(std::move(parentNode));

auto childNode = std::make_unique<SceneNode>(std::move(renderable));
auto* child = parent->addChild(std::move(childNode));

// Transform parent
parent->translate(vec3f(0.0f, 2.0f, 0.0f));

// Child automatically inherits parent's transform
// and applies its own on top
child->rotate(rotationMatrix);
```

## Culling Strategies

The scene graph supports pluggable culling strategies to determine which objects are visible:

- **`CullNoneStrategy`**: Renders all objects (no culling)
- **`FrustumCullingStrategy`**: Only renders objects in camera frustum

```cpp
auto frustumCulling = std::make_unique<FrustumCullingStrategy>();
auto scene = std::make_unique<Scene>(std::move(frustumCulling));
```

## Creating a Snapshot

The scene creates a **snapshot** of visible objects for rendering:

```cpp
// Create immutable snapshot of current scene state
const auto& snapshot = scene->createSnapshot(*camera);

// Snapshot contains:
// - View matrix
// - Projection matrix
// - List of visible renderables with world transforms
```

## Best Practices

1. **Minimize scene graph depth**: Flatten hierarchies where possible for better performance
2. **Update only when needed**: Transformations are cached and only recomputed when changed
3. **Use culling**: Enable frustum culling for large scenes
4. **Batch by material**: Group objects with the same material together

## Example: Solar System

:::note Simplified Example
This example demonstrates the scene graph hierarchy. Renderable creation (sunRenderable, earthRenderable, moonRenderable) and rotation matrix calculations are omitted for clarity. See **[examples/simple_cube_rendering](https://github.com/thorstensuckow/helios/tree/main/examples/simple_cube_rendering)** for complete implementations.
:::

```cpp
// Sun at center
auto sunNode = std::make_unique<SceneNode>(std::move(sunRenderable));
auto* sun = scene->addNode(std::move(sunNode));

// Earth orbits sun
auto earthNode = std::make_unique<SceneNode>(std::move(earthRenderable));
auto* earth = sun->addChild(std::move(earthNode));
earth->translate(vec3f(10.0f, 0.0f, 0.0f));

// Moon orbits earth
auto moonNode = std::make_unique<SceneNode>(std::move(moonRenderable));
auto* moon = earth->addChild(std::move(moonNode));
moon->translate(vec3f(2.0f, 0.0f, 0.0f));

// Animate orbits in game loop
sun->rotate(sunRotation);
earth->rotate(earthRotation);
moon->rotate(moonRotation);
```

## See Also

- [Component System](component-system.md) — ECS architecture with SceneNodeComponent
- [Conventions](conventions.md) — Coordinate systems and matrix storage
- [Spawn System](spawn-system.md) — Entity lifecycle with pooling

