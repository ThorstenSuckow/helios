# Game Loop Architecture

helios distinguishes between **Commands** (world-mutating operations) and **Events** (signals/facts).
Commands exist to mutate the world state _deterministically_. Events exist to decouple systems: they either express a request/intent (e.g. `SpawnCommand`) or a fact (e.g. `SolidCollisionEvent`, `TriggerCollisionEvent`, `SpawnPlanCommandExecutedEvent`).

## Phase/Pass Structure

The game loop is organized into **Phases** and **Passes**:

```
┌─────────────────────────────────────────────────────────────────────┐
│                            FRAME                                    │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  PRE PHASE ──────────────────────────────────────────────────────   │
│    Pass 1 (Input) ──> Pass 2 (Commit) ──> Pass 3                    │
│                            │                                        │
│                     [Pass Commit Point]                             │
│  ────────────────────────────────────────────────── Phase Commit    │
│                                                                     │
│  MAIN PHASE ─────────────────────────────────────────────────────   │
│    Pass 1 (Gameplay) ──> Pass 2 (Collision) ──> Pass 3 (AI)         │
│  ────────────────────────────────────────────────── Phase Commit    │
│                                                                     │
│  POST PHASE ─────────────────────────────────────────────────────   │
│    Pass 1 (Scene Sync) ──> Pass 2 (Cleanup)                         │
│  ────────────────────────────────────────────────── Phase Commit    │
│                                                                     │
│                          RENDER                                     │
└─────────────────────────────────────────────────────────────────────┘
```

**Phases** are major segments (Pre, Main, Post) with distinct responsibilities. After each phase, a **Phase Commit** occurs: phase events become readable, pass events are cleared, commands are flushed, and managers process their queues.

**Passes** are sub-units within phases. A pass can optionally have a **Commit Point**, making pass-level events readable in subsequent passes.

For detailed event propagation rules, see [Event System](event-system.md).

## Practical Example: Twin-Stick Shooter

The following example demonstrates how to configure a complete game loop for a twin-stick shooter with spawning, physics, collision, and rendering:

```cpp
// Create core objects
helios::engine::runtime::gameloop::GameLoop gameLoop{};
helios::engine::runtime::world::GameWorld gameWorld{};

// Register command dispatchers
gameLoop.commandBuffer()
    .addDispatcher<SpawnCommand>(
        std::make_unique<SpawnCommandDispatcher>())
    .addDispatcher<DespawnCommand>(
        std::make_unique<DespawnCommandDispatcher>())
    .addDispatcher<ScheduledSpawnPlanCommand>(
        std::make_unique<ScheduledSpawnPlanCommandDispatcher>());

// ═══════════════════════════════════════════════════════════════════
// PRE PHASE: Input, Spawning, Motion Preparation
// ═══════════════════════════════════════════════════════════════════
gameLoop.phase(PhaseType::Pre)
    // Pass 1: Input handling
    .addPass()
    .addSystem<TwinStickInputSystem>(*playerGameObject)
    
    // Commit Point: Structural changes (spawn/despawn) execute here
    .addCommitPoint(CommitPoint::Structural)
    
    // Pass 2: Spawn scheduling (after input, before physics)
    .addPass()
    .addSystem<GameObjectSpawnSystem>(spawnSchedulers)
    
    // Commit Point: New entities are now active
    .addCommitPoint(CommitPoint::Structural)
    
    // Pass 3: Motion systems (all entities including newly spawned)
    .addPass()
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<SpinSystem>()
    .addSystem<Move2DSystem>();

// ═══════════════════════════════════════════════════════════════════
// MAIN PHASE: Gameplay Logic, Collision, AI
// ═══════════════════════════════════════════════════════════════════
gameLoop.phase(PhaseType::Main)
    // Pass 1: Update bounds and check collisions
    .addPass()
    .addSystem<BoundsUpdateSystem>()
    .addSystem<LevelBoundsBehaviorSystem>()
    .addSystem<GridCollisionDetectionSystem>(cellSize, levelBounds)
    
    // Commit Point: Collision events become readable
    .addCommitPoint()
    
    // Pass 2: React to collisions (damage, despawn, etc.)
    .addPass()
    .addSystem<ProjectileCollisionSystem>()
    .addSystem<EnemyCollisionSystem>();

// ═══════════════════════════════════════════════════════════════════
// POST PHASE: Scene Sync, Transform Cleanup
// ═══════════════════════════════════════════════════════════════════
gameLoop.phase(PhaseType::Post)
    .addPass()
    .addSystem<ComposeTransformSystem>()
    .addSystem<SceneSyncSystem>(scene.get())
    .addSystem<TransformClearSystem>()
    .addSystem<DelayedComponentEnablerSystem>();

// Initialize and run
gameWorld.init();
gameLoop.init(gameWorld);

// Main loop
while (running) {
    float deltaTime = /* calculate delta */;
    
    UpdateContext ctx{deltaTime, gameWorld, /* ... */};
    gameLoop.update(ctx);
    
    render();
}
```

### Execution Flow for One Frame

The following diagram shows how a projectile collision is processed:

```
Frame N
═══════════════════════════════════════════════════════════════════════

PRE PHASE
┌─────────────────────────────────────────────────────────────────────┐
│ Pass 1: TwinStickInputSystem                                        │
│   → Player presses fire button                                      │
│   → Pushes ShootCommand to CommandBuffer                            │
├─────────────────────────────────────────────────────────────────────┤
│ [Commit Point: Structural]                                          │
│   → CommandBuffer.flush() executes ShootCommand                     │
│   → SpawnManager creates projectile from pool                       │
├─────────────────────────────────────────────────────────────────────┤
│ Pass 2: GameObjectSpawnSystem                                       │
│   → Evaluates spawn rules, schedules enemy spawns                   │
├─────────────────────────────────────────────────────────────────────┤
│ [Commit Point: Structural]                                          │
│   → New enemies spawned and activated                               │
├─────────────────────────────────────────────────────────────────────┤
│ Pass 3: Move2DSystem, SteeringSystem                                │
│   → All entities (including new projectile) move                    │
└─────────────────────────────────────────────────────────────────────┘
                              ↓
                      [Phase Commit]
                              ↓
MAIN PHASE
┌─────────────────────────────────────────────────────────────────────┐
│ Pass 1: GridCollisionDetectionSystem                                │
│   → Detects projectile-enemy collision                              │
│   → Pushes TriggerCollisionEvent to Pass EventBus                   │
├─────────────────────────────────────────────────────────────────────┤
│ [Commit Point]                                                      │
│   → Pass events become readable                                     │
├─────────────────────────────────────────────────────────────────────┤
│ Pass 2: ProjectileCollisionSystem                                   │
│   → Reads TriggerCollisionEvent                                     │
│   → Pushes DespawnCommand for projectile                            │
│   → Pushes DamageCommand for enemy                                  │
└─────────────────────────────────────────────────────────────────────┘
                              ↓
                      [Phase Commit]
                              ↓
POST PHASE
┌─────────────────────────────────────────────────────────────────────┐
│ Pass 1: SceneSyncSystem                                             │
│   → Syncs transforms to scene graph                                 │
│   → Despawned projectile removed from scene                         │
└─────────────────────────────────────────────────────────────────────┘
                              ↓
                      [Phase Commit]
                      [Frame Commit]
                              ↓
                          RENDER
```

### Key Observations

1. **Commit Points control visibility**: The projectile spawned in Pre Phase Pass 1 is immediately available for physics in Pass 3 because of the structural commit point.

2. **Pass events enable same-phase reactions**: The collision detected in Main Phase Pass 1 is readable in Pass 2, allowing the collision response in the same phase.

3. **Commands execute at phase boundaries**: The `DespawnCommand` pushed in Main Phase executes at the Main Phase Commit, so the entity is removed before rendering.

4. **Deterministic ordering**: Systems within a pass execute in registration order. This ensures predictable behavior across frames.

## Commands and CommandBuffer

Systems can write Commands into the CommandBuffer during any phase. At each **Phase Commit**, the CommandBuffer is flushed — i.e., their `execute()` method is invoked. This method contains the logic that mutates the world state (e.g., spawning, despawning, health changes, component changes).

This means commands added during the Pre Phase are executed at the Pre Phase Commit, commands added during Main Phase at Main Phase Commit, and so on. This allows for responsive gameplay where actions taken in one phase are immediately visible in the next.

Commands are "bare metal" and therefore the lowest level in the game-loop layer — no further preparation of a Command is required. The system should therefore also be able to commit Commands coming directly from a developer console into the GameWorld (optionally delegating them to their respective managers — see below).

For detailed command handling, dispatchers, and manager integration, see [Command System](command-system.md).

## Events and double-buffered EventBus

In addition, systems can emit Events in frame N, e.g. **request events** - events that intend to mutate the world state - or plain signals such as `SolidCollisionEvent`, from which world-mutating Commands (despawn) can be derived.

helios provides three event buses with different visibility scopes:

| Event Bus | Push Method | Read Method | Visibility |
|-----------|-------------|-------------|------------|
| **Pass** | `pushPass<E>()` | `readPass<E>()` | Subsequent passes (same phase) |
| **Phase** | `pushPhase<E>()` | `readPhase<E>()` | Next phase |
| **Frame** | `pushFrame<E>()` | `readFrame<E>()` | Next frame |

All buses are double-buffered (`helios.core.buffer.TypeIndexedDoubleBuffer`): events are written into the write buffer and become visible in the read buffer only after a buffer swap.

For detailed event propagation rules and commit points, see [Event System](event-system.md).

## Frame Order Summary

At each phase boundary, the following commit sequence occurs:

```cpp
// After each phase completes
phaseEventBus.swapBuffers();    // Phase events become readable
passEventBus.clearAll();         // Pass events are cleared
commandBuffer.flush();           // Commands execute (mutations)
gameWorld.flushManagers();       // Managers process queued requests

// Additionally, at the end of Post phase:
frameEventBus.swapBuffers();     // Frame events become readable in next frame
```

Overall frame execution:

```cpp
for (phase : {Pre, Main, Post}) {
    
    for (pass : phase.passes()) {
        for (system : pass.systems()) {
            system.update(updateContext);
        }
        
        if (pass.commitPoint() == CommitPoint::PassEvents) {
            passEventBus.swapBuffers();  // Pass events readable
        }
    }
    
    phaseCommit();  // Phase boundary
    
    if (phase == Post) {
        frameEventBus.swapBuffers();  // Frame events readable in next frame
    }
}

render();
```

## Immediate Events (single-buffered)

For time-critical feedback that does not mutate the world state (particle effects, audio feedback), there is a separate **ImmediateBus** (single-buffered).
Immediate events should be processed within the same frame without additional double-buffer latency. A suitable dispatch point is after `CommandBuffer.flush()`, so that feedback can observe the world that was committed at the beginning of the frame in a consistent state. This mitigates latency (e.g. ~16ms at 60fps) and allows feedback to be triggered in the same frame as a specific event, which improves the overall user experience ("game feel").

## Related Documentation

- [Command System](command-system.md) — Command pattern, dispatchers, managers
- [Event System](event-system.md) — Phase/Pass event propagation with double-buffered buses
- [Component System](component-system.md) — ECS-style composition architecture
- [Spawn System](spawn-system.md) — Entity lifecycle with spawn scheduling and pooling
