# helios::engine::modules::scene

Scene graph integration for game entities.

This module provides components and systems that bridge the game logic layer (GameObjects) with the rendering layer (SceneNodes).

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Scene node reference components |
| `systems/` | Transform synchronization with scene graph |

## Key Classes

| Class | Purpose |
|-------|---------|
| `SceneNodeComponent` | Holds reference to the entity's SceneNode |
| `SceneSyncSystem` | Syncs composed transforms to SceneNodes each frame |

## Architecture

The scene module bridges two layers:

- **Game Layer**: GameObjects with ComposeTransformComponent
- **Render Layer**: SceneNodes with world transforms

`SceneSyncSystem` copies the composed transform matrix from each GameObject to its associated SceneNode, keeping the visual representation in sync with game state.

## Usage

```cpp
// Create scene node for entity
auto sceneNode = scene.rootNode().createChild();

// Create entity with scene integration
auto player = GameObjectFactory::gameObject()
    .withRendering([&](auto& r) {
        r.renderable()
         .shape(triangleShape)
         .shader(shader)
         .attachTo(sceneNode);  // Links to SceneNode
    })
    .make(true);

// SceneNodeComponent is added automatically by RenderingBuilder
// Manual access if needed:
auto* snc = player->get<SceneNodeComponent>();
helios::scene::SceneNode* node = snc->sceneNode();

// Register sync system in Post phase
gameLoop.phase(PhaseType::Post).addPass()
    .addSystem<SceneSyncSystem>(&gameWorld, &scene);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::scene
@brief Scene graph integration for game entities.
@details This module provides components and systems that bridge the game logic layer (GameObjects) with the rendering layer (SceneNodes).
</p></details>

