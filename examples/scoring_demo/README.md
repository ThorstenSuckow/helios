# Scoring Demo

This example demonstrates the **scoring system** in helios, including score pools, score observers, UI text rendering, state management, and integration with the combat/health/spawn mechanics.

## Features

- **Score Pool Management** - Centralized score tracking with `ScorePoolManager`
- **Score Attribution** - Kill scoring via `LastAttackerComponent` and `ScorePoolComponent`
- **UI Text Rendering** - Dynamic score display with `OpenGLGlyphTextRenderer` and `TextMesh`
- **Score Observers** - Data binding between score pools and UI components
- **Health & Damage System** - Enemy health tracking with `HealthComponent`
- **Game / Match State Management** - Hierarchical state machine with `GameState` and `MatchState`
- **Menu System** - Title screen, pause menu, and game over menu with gamepad navigation
- **SpawnSystemFactory DSL** - Declarative spawn configuration with fluent builder API
- **Multiple Viewports** - Separate viewports for game world, HUD, title screen, and menus
- **Complete Twin-Stick Gameplay** - Player movement, aiming, shooting, and scoring

## Building

```bash
cmake -S . -B build
cmake --build build --target scoring_demomain
```

## Running

```bash
./build/examples/scoring_demo/main
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship |
| Right Stick | Aim and fire projectiles |
| D-Pad Up/Down | Navigate menus |
| A Button | Confirm menu selection |
| ESC | Exit application |
| ~ (Tilde) | Toggle ImGui overlay |

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
| `main.cpp` | Application entry point with game loop and phase configuration |
| `_module.ixx` | Module aggregation |
| `ArenaConfig.ixx` | Arena constants (grid size, coordinates, cell length) |
| `IdConfig.ixx` | Centralized typed identifiers (pool, prefab, spawn, collision IDs) |
| `CollisionId.ixx` | Collision layer identifiers |
| `EnemyPrefabs.ixx` | Enemy prefab definitions (purple, orange, blue) |
| `SpawnConfiguration.ixx` | `SpawnSystemFactory` DSL configuration for all spawn pools |
| `MenuConfiguration.ixx` | Title, pause, and game over menu setup |
| `GameStateListener.ixx` | Game state transition listeners |
| `MatchStateListener.ixx` | Match state transition listeners |
| `UiActionCommandPolicy.ixx` | UI action command handling |
| `SpaceshipWidget.ixx` | ImGui widget for physics and fire rate tuning |

### Key Components

| Component | Purpose |
|-----------|---------|
| `ScorePoolComponent` | Associates player with a score pool |
| `ScoreValueComponent` | Points awarded when entity is defeated |
| `ScoreObserverComponent` | Observes a score pool for UI updates |
| `LastAttackerComponent` | Tracks who dealt the killing blow |
| `HealthComponent` | Entity health and damage tracking |
| `MatchStateComponent` | Per-entity match state tracking |
| `MenuComponent` | Menu state and item configuration |
| `UiTextComponent` | Text rendering in UI context |

### Key Systems

| System | Purpose |
|--------|---------|
| `HealthDepletedSystem` | Detects entity death, triggers score award |
| `ScoreObserverSystem` | Updates observers when pools change |
| `DamageApplicationSystem` | Applies collision damage |
| `GameFlowSystem` | Manages game state transitions |
| `MatchFlowSystem` | Manages match state transitions |
| `MenuDisplaySystem` | State-driven menu visibility |
| `MenuNavigationSystem` | Gamepad-based menu navigation |
| `StateToViewportPolicyUpdateSystem` | Activates viewports based on state |
| `SceneRenderingSystem` | Renders scenes to their associated viewports |

## Spawn System Configuration (DSL)

```cpp
SpawnSystemFactory::configure(poolManager, spawnManager)
    .pool(ProjectilePoolId, ProjectilePrefabId, 50)
        .profile(ProjectileSpawnProfileId)
            .emitterPlacement()
            .done()
        .commit()

    .pool(PurpleEnemyPoolId, PurpleEnemyPrefabId, 50)
        .profile(RandomSpawnProfileId)
            .randomPlacement()
            .randomDirectionInitializer()
            .scheduledBy(PurpleSpawnRuleId)
                .timerCondition(5.0f)
                .fixedAmount(1)
                .done()
            .done()
        .commit()

    .pool(OrangeEnemyPoolId, OrangeEnemyPrefabId, OBJECT_AMOUNT_X)
        .profile(LeftColumnProfileId)
            .axisPlacement(vec3f(0, -1, 0).normalize(), TOP_LEFT)
            .moveInitializer(X_AXISf)
            .scheduledBy(LeftColumnRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(OBJECT_AMOUNT_Y)
                .done()
            .done()
        // ... additional profiles for other edges
        .commitCyclic<4>();
```

## GameLoop Phase Configuration

```cpp
// Pre-Phase: Input, Spawning, Physics
gameLoop.phase(PhaseType::Pre)
    .addPass<GameState>(GameState::Running)
    .addSystem<TwinStickInputSystem>(shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass<GameState>(GameState::Running)
    .addSystem<GameObjectSpawnSystem>(spawnManager)
    .addSystem<ProjectileSpawnSystem>(ProjectileSpawnProfileId)
    .addCommitPoint(CommitPoint::Structural);

// Main-Phase: Collision Detection and Response
gameLoop.phase(PhaseType::Main)
    .addPass<GameState>(GameState::Running)
    .addSystem<GridCollisionDetectionSystem>(bounds, cellSize)
    .addCommitPoint();

// Post-Phase: Transform, Rendering, UI
gameLoop.phase(PhaseType::Post)
    .addPass<GameState>(GameState::Any)
    .addSystem<ComposeTransformSystem>()
    .addSystem<StateToViewportPolicyUpdateSystem<GameState, MatchState>>(stateToViewportMap)
    .addSystem<SceneRenderingSystem>(renderingDevice, sceneToViewportMap)
    .addSystem<TransformClearSystem>();
```

## State Management

The demo uses a hierarchical state machine:

- **GameState**: `Undefined` → `Start` → `Running` → `Paused` / `GameOver`
- **MatchState**: `Undefined` → `WaitingForPlayers` → `Playing` → `GameOver`

State transitions drive viewport visibility, system execution, and menu display.

## See Also

- [State Management](../../docs/core-concepts/state-management.md) - Hierarchical state machine architecture
- [Spawn System](../../docs/core-concepts/spawn-system.md) - Spawn pipeline and DSL builder
- [Object Pooling](../../docs/core-concepts/object-pooling.md) - Entity pooling system
- [Collision Detection Example](../collision_detection/README.md) - Grid-based collision
- [Enemy Spawn Example](../enemy_spawn/README.md) - Spawn system with pooling
