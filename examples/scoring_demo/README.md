# Scoring Demo

This example demonstrates the **scoring system** in helios, including score pools, score observers, UI text rendering, and integration with the combat/health mechanics.

## Features

- **Score Pool Management** - Centralized score tracking with `ScorePoolManager`
- **Score Attribution** - Kill scoring via `LastAttackerComponent` and `ScorePoolComponent`
- **UI Text Rendering** - Dynamic score display with `OpenGLGlyphTextRenderer`
- **Score Observers** - Data binding between score pools and UI components
- **Health & Damage System** - Enemy health tracking with `HealthComponent`
- **Complete Twin-Stick Gameplay** - Player movement, aiming, and shooting

## Building

```bash
cmake -S . -B build
cmake --build build --target scoring_demo
```

## Running

```bash
./build/bin/scoring_demo
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship |
| Right Stick | Aim and fire projectiles |
| ESC | Exit application |

## Architecture

### Scoring System Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                      SCORING ARCHITECTURE                           │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ENEMY DEFEATED                                                     │
│  ┌─────────────────┐                                                │
│  │ HealthComponent │ → health <= 0                                  │
│  │ ScoreValueComp. │ → points = 100                                 │
│  │ LastAttackerCmp │ → attacker = player                            │
│  └────────┬────────┘                                                │
│           │                                                         │
│           ▼                                                         │
│  ATTACKER LOOKUP                                                    │
│  ┌─────────────────┐                                                │
│  │ Player Entity   │                                                │
│  │ ScorePoolComp.  │ → poolId = "playerOneScorePool"                │
│  └────────┬────────┘                                                │
│           │                                                         │
│           ▼                                                         │
│  SCORE POOL UPDATE                                                  │
│  ┌─────────────────┐                                                │
│  │ ScorePoolManager│ → addScore(poolId, 100)                        │
│  │ ScorePool       │ → totalScore += 100, revision++                │
│  └────────┬────────┘                                                │
│           │                                                         │
│           ▼                                                         │
│  UI UPDATE                                                          │
│  ┌─────────────────┐                                                │
│  │ScoreObserverCmp │ → detects revision change                      │
│  │TextMeshComponent│ → updates displayed text                       │
│  └─────────────────┘                                                │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point with scoring setup |
| `CollisionId.ixx` | Collision layer identifiers |
| `_module.ixx` | Module aggregation |

### Key Components

| Component | Purpose |
|-----------|---------|
| `ScorePoolComponent` | Associates player with a score pool |
| `ScoreValueComponent` | Points awarded when entity is defeated |
| `ScoreObserverComponent` | Observes a score pool for UI updates |
| `LastAttackerComponent` | Tracks who dealt the killing blow |
| `HealthComponent` | Entity health and damage tracking |
| `TextMeshComponent` | Renders dynamic text |

### Key Systems

| System | Purpose |
|--------|---------|
| `HealthDepletedSystem` | Detects entity death, triggers score award |
| `ScoreObserverSystem` | Updates observers when pools change |
| `TextMeshUpdateSystem` | Updates text mesh from bound data |
| `DamageApplicationSystem` | Applies collision damage |

## Score Pool Setup

```cpp
// Create score pool manager
auto& scorePoolManager = gameWorld.addManager<ScorePoolManager>();

// Create a score pool for player one
scorePoolManager.addScorePool(ScorePoolId{"playerOneScorePool"});

// Register score command handler
gameWorld.registerScoreCommandHandler(scorePoolManager);
```

## Player Configuration

```cpp
// Associate player with score pool
auto player = GameObjectFactory::gameObject(gameWorld)
    .withScoring([](auto& scoring) {
        scoring.scorePool()
               .poolId(ScorePoolId{"playerOneScorePool"});
    })
    .withCombat([](auto& combat) {
        combat.attacker();  // Enables LastAttackerComponent tracking
    })
    .make(true);
```

## Enemy Configuration

```cpp
// Configure enemy with score value
auto enemy = GameObjectFactory::gameObject(gameWorld)
    .withHealth([](auto& health) {
        health.health()
              .maxHealth(100.0f);
    })
    .withScoring([](auto& scoring) {
        scoring.scoreValue<EnemyScore>()
               .points(100);
    })
    .withCombat([](auto& combat) {
        combat.trackLastAttacker();  // Track who kills this enemy
    })
    .make(true);
```

## Score UI Setup

```cpp
// Create score text display
auto scoreText = GameObjectFactory::gameObject(gameWorld)
    .withRendering([&](auto& r) {
        r.textRenderable()
         .font(fontId)
         .text("Score: 0")
         .build();
    })
    .withUiTransform([&](auto& ui) {
        ui.transform()
          .anchor(Anchor::TopLeft)
          .offsets({10.0f, 10.0f, 0.0f, 0.0f})
          .viewportId(mainViewportId);
    })
    .withObserver([](auto& obs) {
        obs.scoreObserver()
           .poolId(ScorePoolId{"playerOneScorePool"})
           .formatter([](double score) {
               return std::format("Score: {}", static_cast<int>(score));
           });
    })
    .make(true);
```

## GameLoop Configuration

```cpp
// Main Phase: Combat and Scoring
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<DamageApplicationSystem>()
    .addSystem<HealthDepletedSystem>()
    .addCommitPoint()
    .addPass()
    .addSystem<ScoreObserverSystem>(&scorePoolManager);

// Post Phase: UI Updates
gameLoop.phase(PhaseType::Post)
    .addPass()
    .addSystem<TextMeshUpdateSystem>()
    .addSystem<UiTransformSystem>();
```

## Score Types

Define custom score types for different point values:

```cpp
// Custom score type
struct EnemyScore : public Score {
    int points = 100;
    
    ScoreTypeId typeId() const override {
        return ScoreTypeId{"enemy"};
    }
    
    double value() const override {
        return static_cast<double>(points);
    }
};

// Use with ScoreValueComponent
entity.add<ScoreValueComponent<EnemyScore>>(100);
```

## See Also

- [Text Rendering](../../docs/core-concepts/text-rendering.md) - FreeType-based text rendering
- [Component Lifecycle](../../docs/core-concepts/component-lifecycle.md) - Lifecycle hooks (onAcquire, onRelease)
- [Object Pooling](../../docs/core-concepts/object-pooling.md) - Entity pooling system
- [Collision Detection Example](../collision_detection/README.md) - Grid-based collision with damage
- [Enemy Spawn Example](../enemy_spawn/README.md) - Spawn system with pooling
