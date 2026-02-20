# Spawn System

helios provides a comprehensive **spawn system** for managing entity lifecycle at runtime. The system separates concerns into scheduling (when/how many), behavior (where/how), and execution (pool integration). A fluent **SpawnSystemFactory** builder provides ID-centric configuration where each pool ID is the root for all related profiles and scheduling rules.

## Overview

The spawn system consists of several layers working together:

- **SpawnScheduler:** Evaluates rules and determines when spawning should occur
- **SpawnProfile:** Defines how entities are spawned (pool, placer, initializer)
- **SpawnManager:** Processes spawn/despawn commands via pools
- **Commands & Dispatchers:** Route commands through the command pipeline
- **SpawnCommandHandler:** Interface for managers that process spawn commands
- **SpawnSystemFactory:** Fluent builder for ID-centric spawn configuration

```
┌─────────────────────────────────────────────────────────────────────┐
│                         SPAWN PIPELINE                              │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  CONFIGURATION LAYER (SpawnSystemFactory)                           │
│  ┌─────────────────────────────────────────┐                        │
│  │  pool(PoolId, prefab, size)             │                        │
│  │    └─ profile(ProfileId)                │                        │
│  │         ├─ placement + initializer      │                        │
│  │         └─ scheduledBy(RuleId)          │                        │
│  │              └─ condition + amount      │                        │
│  └─────────────────────────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  POLICY LAYER (when/how many)                                       │
│  ┌─────────────┐    ┌──────────────┐   ┌──────────────┐             │
│  │ SpawnRule   │───>│SpawnCondition───>│AmountProvider│             │
│  └─────────────┘    └──────────────┘   └──────────────┘             │
│         │                                                           │
│         ▼                                                           │
│  SCHEDULING LAYER                                                   │
│  ┌─────────────────────────────────────────┐                        │
│  │           SpawnScheduler                │                        │
│  │  ┌──────────────────┐ ┌───────────────┐ │                        │
│  │  │DefaultScheduler  │ │CyclicScheduler│ │                        │
│  │  └──────────────────┘ └───────────────┘ │                        │
│  └────────────────────┬────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  COMMAND LAYER                                                      │
│  ┌─────────────────────────────────────────┐                        │
│  │     ScheduledSpawnPlanCommand           │                        │
│  └────────────────────┬────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  EXECUTION LAYER                                                    │
│  ┌─────────────────────────────────────────┐                        │
│  │           SpawnManager                  │                        │
│  │  ┌───────────┐    ┌───────────┐         │                        │
│  │  │SpawnPlacer│    │Initializer│         │                        │
│  │  └───────────┘    └───────────┘         │                        │
│  └─────────────────────────────────────────┘                        │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Key Concepts

### SpawnProfile

A `SpawnProfile` bundles together everything needed to spawn entities of a particular type:

```cpp
import helios.engine.runtime.spawn.SpawnProfile;
import helios.engine.runtime.spawn.behavior.placements.RandomSpawnPlacer;
import helios.engine.runtime.spawn.behavior.initializers.RandomDirectionInitializer;

auto enemyProfile = std::make_unique<SpawnProfile>(SpawnProfile{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer>(),
    .spawnInitializer = std::make_unique<RandomDirectionInitializer>()
});
```

| Field | Purpose |
|-------|---------|
| `gameObjectPoolId` | Which pool to acquire entities from |
| `spawnPlacer` | Strategy for positioning spawned entities |
| `spawnInitializer` | Strategy for initializing entity components |

### SpawnPlacer

Determines **where** entities spawn. Built-in implementations:

| Placer | Behavior |
|--------|----------|
| `RandomSpawnPlacer` | Random position within level bounds |
| `EmitterSpawnPlacer` | At the emitter's current position |
| `AxisSpawnPlacer` | Distributed along an axis from an origin |
| `ColumnSpawnPlacer` | Vertical column formation |
| `DistributedSpawnPlacer<N>` | Evenly distributed across N anchor points |

```cpp
class RandomSpawnPlacer : public SpawnPlacer {
public:
    vec3f getPosition(const SpawnContext& ctx) const override {
        const auto& bounds = ctx.level().bounds();
        return vec3f{
            Random::range(bounds.min()[0], bounds.max()[0]),
            Random::range(bounds.min()[1], bounds.max()[1]),
            0.0f
        };
    }
};
```

### SpawnInitializer

Configures spawned entities with initial state. Built-in implementations:

| Initializer | Behavior |
|-------------|----------|
| `EmitterInitializer` | Copies direction/velocity from emitter |
| `MoveInitializer` | Sets movement direction (fixed, random, or point) |
| `RandomDirectionInitializer` | Assigns random 2D direction |
| `DelayedComponentEnablerInitializer<T>` | Enables a component after a delay |
| `InitializerList<N>` | Chains multiple initializers sequentially |

```cpp
class EmitterInitializer : public SpawnInitializer {
public:
    void initialize(GameObject& entity, const SpawnContext& ctx) const override {
        if (const auto& emitter = ctx.emitterContext()) {
            if (auto* dir = entity.get<DirectionComponent>()) {
                dir->setDirection(emitter->direction);
            }
        }
    }
};
```

## Spawn Rules and Scheduling

### SpawnRule

A `SpawnRule` combines a **condition** (when to spawn) with an **amount provider** (how many):

```cpp
import helios.engine.runtime.spawn.policy.SpawnRule;
import helios.engine.runtime.spawn.policy.conditions.TimerSpawnCondition;
import helios.engine.runtime.spawn.policy.amount.FixedSpawnAmount;

auto rule = std::make_unique<SpawnRule>(
    std::make_unique<TimerSpawnCondition>(2.0f),  // Every 2 seconds
    std::make_unique<FixedSpawnAmount>(3),        // Spawn 3 entities
    SpawnRuleId{1}
);
```

### SpawnCondition

Determines **when** spawning should occur:

| Condition | Behavior |
|-----------|----------|
| `TimerSpawnCondition` | Triggers at fixed time intervals |
| `RequestedAmountIsAvailableCondition` | Triggers only if the pool has enough inactive entities |
| `SpawnConditionAll` | Composite: all child conditions must be satisfied |

```cpp
class TimerSpawnCondition : public SpawnCondition {
public:
    explicit TimerSpawnCondition(float interval) : interval_(interval) {}
    
    bool isSatisfied(size_t amount, const SpawnRuleState& state,
                     GameObjectPoolSnapshot pool, const UpdateContext& ctx) const override {
        return state.sinceLastSpawn() >= interval_ 
            && pool.inactiveCount >= amount;
    }
    
    void onCommit(SpawnRuleState& state, size_t count) const override {
        state.setSinceLastSpawn(0.0f);
    }
};
```

### SpawnAmountProvider

Determines **how many** entities to spawn:

| Provider | Behavior |
|----------|----------|
| `FixedSpawnAmount` | Always returns a fixed count |
| `SpawnAll` | Returns the total available count from the pool |
| `SpawnAmountByCallback` | Delegates to a user-provided function |

```cpp
// Dynamic amount based on game state
auto dynamicAmount = std::make_unique<SpawnAmountByCallback>(
    [](const GameObjectPoolId& poolId, const SpawnRuleState& state,
       const UpdateContext& ctx) -> size_t {
        return ctx.gameWorld().difficultyLevel() + 1;
    }
);
```

### SpawnScheduler

The `SpawnScheduler` evaluates all registered rules each frame. helios ships with two scheduler implementations:

| Scheduler | Behavior |
|-----------|----------|
| `DefaultSpawnScheduler` | Evaluates all rules every frame; independent triggers |
| `CyclicSpawnScheduler<N>` | Round-robin through N rules; advances only on successful spawn |

#### DefaultSpawnScheduler

Evaluates all rules independently each frame. Each rule can trigger on its own schedule:

```cpp
import helios.engine.runtime.spawn.scheduling.DefaultSpawnScheduler;

auto scheduler = std::make_unique<DefaultSpawnScheduler>();
scheduler->addRule(enemyProfileId, std::move(rule));
```

#### CyclicSpawnScheduler

Evaluates one rule at a time in a fixed-size ring buffer. Advances to the next rule only when a spawn successfully occurs. Ideal for wave-based or sequential spawn patterns:

```cpp
import helios.engine.runtime.spawn.scheduling.CyclicSpawnScheduler;

// Cycle: Left → Top → Right → Bottom → Left → ...
auto scheduler = std::make_unique<CyclicSpawnScheduler<4>>();
scheduler->addRule(leftProfileId,   std::move(leftRule));
scheduler->addRule(topProfileId,    std::move(topRule));
scheduler->addRule(rightProfileId,  std::move(rightRule));
scheduler->addRule(bottomProfileId, std::move(bottomRule));
```

In the game loop, the scheduler produces plans that become commands:

```cpp
import helios.engine.runtime.spawn.scheduling.SpawnScheduler;

SpawnScheduler scheduler;

scheduler.addRule(enemyProfileId, std::move(rule));

// In GameObjectSpawnSystem::update()
scheduler.evaluate(updateContext);
auto plans = scheduler.drainScheduledPlans();

// Convert plans to commands
for (const auto& plan : plans) {
    commandBuffer.add<ScheduledSpawnPlanCommand>(std::move(plan));
}
```

## Command Pipeline Integration

### Commands

Spawn operations are represented as commands for deferred execution:

| Command | Purpose |
|---------|---------|
| `SpawnCommand` | Request to spawn entities from a profile |
| `DespawnCommand` | Request to return an entity to its pool |
| `ScheduledSpawnPlanCommand` | Execute a scheduled spawn plan |

### Dispatchers

Commands are routed to managers via typed dispatchers:

```cpp
import helios.engine.runtime.spawn.dispatcher.SpawnCommandDispatcher;
import helios.engine.runtime.spawn.dispatcher.DespawnCommandDispatcher;
import helios.engine.runtime.spawn.dispatcher.ScheduledSpawnPlanCommandDispatcher;

commandBuffer.addDispatcher<SpawnCommandDispatcher>();
commandBuffer.addDispatcher<DespawnCommandDispatcher>();
commandBuffer.addDispatcher<ScheduledSpawnPlanCommandDispatcher>();
```

### SpawnManager

The `SpawnManager` processes spawn/despawn commands:

```cpp
import helios.engine.runtime.spawn.SpawnManager;

auto spawnManager = std::make_unique<SpawnManager>();

// Register profiles
spawnManager->addSpawnProfile(enemyProfileId, std::move(enemyProfile));
spawnManager->addSpawnProfile(bulletProfileId, std::move(bulletProfile));

// Add to GameWorld
gameWorld.addManager(std::move(spawnManager));
```

## SpawnSystemFactory (Builder)

The `SpawnSystemFactory` provides a fluent, **ID-centric** builder for configuring the entire spawn system. Each pool ID serves as the entry point for its associated profiles and scheduling rules. This co-locates related configuration and eliminates the risk of mismatched IDs.

### Builder Hierarchy

```
SpawnSystemFactory::configure(poolManager, spawnManager)  →  SpawnSystemConfigurator
    │
    └─ pool(PoolId, prefab, size)  →  SpawnPoolConfig
         │
         ├─ profile(ProfileId)  →  SpawnProfileConfig
         │    │
         │    ├─ emitterPlacement() / randomPlacement() / axisPlacement(...)
         │    ├─ moveInitializer(...) / randomDirectionInitializer()
         │    ├─ placer(custom) / initializer(custom)
         │    │
         │    ├─ scheduledBy(RuleId)  →  SpawnRuleConfig
         │    │    ├─ timerCondition(interval)
         │    │    ├─ timerWithAvailabilityCondition(interval)
         │    │    ├─ fixedAmount(count)
         │    │    └─ done()  →  back to SpawnProfileConfig
         │    │
         │    └─ done()  →  back to SpawnPoolConfig
         │
         ├─ profile(...)  →  additional profiles on the same pool
         │
         ├─ commit()              →  SpawnSystemConfigurator (per-profile schedulers)
         ├─ commitCyclic<N>()     →  SpawnSystemConfigurator (one CyclicSpawnScheduler)
         └─ commitProfilesOnly()  →  SpawnSystemConfigurator (no schedulers)
```

`commit()`, `commitCyclic<N>()`, and `commitProfilesOnly()` all return a `SpawnSystemConfigurator` — a lightweight handle holding the manager references. This enables **chained pool configuration** without intermediate variables.

### Design: Callbacks vs. done()-Chaining

The `SpawnSystemFactory` uses **done()-chaining** rather than the callback pattern used by `GameObjectFactory`. This is because spawn configuration has a **natural hierarchy** (pool → profile → rule), where done()-chaining expresses parent-child relationships clearly.

The `GameObjectFactory` uses callbacks because its domains (motion, collision, rendering) are **flat and independent** — callbacks scope each domain without implying hierarchy. See [GameObject Builder](gameobject-builder.md) for details.

| Pattern | Best For | Used By |
|---------|----------|---------|
| done()-chaining | Hierarchical config (parent → child → grandchild) | `SpawnSystemFactory` |
| Callback-based | Flat, independent domains with external captures | `GameObjectFactory` |

### commit() vs. commitCyclic\<N\>() vs. commitProfilesOnly()

| Method | Scheduler Created | Returns | Use Case |
|--------|-------------------|---------|----------|
| `commit()` | One `DefaultSpawnScheduler` per profile with rules | `SpawnSystemConfigurator` | Independent spawn timers |
| `commitCyclic<N>()` | One `CyclicSpawnScheduler<N>` for all profiles | `SpawnSystemConfigurator` | Sequential wave patterns |
| `commitProfilesOnly()` | None | `SpawnSystemConfigurator` | Profiles without scheduling (attach rules separately) |

### Example: Chained Multi-Pool Configuration

The complete spawn system can be configured in a single chained expression.
Each `commit()` / `commitCyclic<N>()` returns a `SpawnSystemConfigurator` that
starts the next `pool()`:

```cpp
using namespace helios::engine::builder::spawnSystem;

SpawnSystemFactory::configure(poolManager, spawnManager)

    // 1. Projectile pool — emitter-relative, no scheduling
    .pool(ProjectilePoolId, projectilePrefab, 50)
        .profile(ProjectileProfileId)
            .emitterPlacement()
            .done()
        .commit()

    // 2. Enemy pool — random placement, timer-scheduled
    .pool(EnemyPoolId, enemyPrefab, 200)
        .profile(EnemyProfileId)
            .randomPlacement()
            .randomDirectionInitializer()
            .scheduledBy(EnemyRuleId)
                .timerCondition(5.0f)
                .fixedAmount(1)
                .done()
            .done()
        .commit()

    // 3. Boss pool — mass spawn with custom placer/initializer
    .pool(BossPoolId, bossPrefab, 10)
        .profile(BossProfileId)
            .placer(std::make_unique<DistributedSpawnPlacer<2>>(left, right))
            .initializer(std::make_unique<InitializerList<2>>(
                std::make_unique<DelayedComponentEnablerInitializer<Move2DComponent>>(0.5f, 25),
                std::make_unique<MoveInitializer>(vec3f{0}, DirectionType::Point)
            ))
            .scheduledBy(BossRuleId)
                .timerWithAvailabilityCondition(30.0f)
                .fixedAmount(2)
                .done()
            .done()
        .commit()

    // 4. Edge enemy pool — cyclic 4-direction spawning
    .pool(EdgeEnemyPoolId, edgePrefab, 100)
        .profile(LeftProfileId)
            .axisPlacement(vec3f(0, -1, 0).normalize(), topLeft)
            .moveInitializer(X_AXISf)
            .scheduledBy(LeftRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(19)
                .done()
            .done()
        .profile(TopProfileId)
            .axisPlacement(vec3f(1, 0, 0).normalize(), topLeft)
            .moveInitializer(Y_AXISf * -1.0f)
            .scheduledBy(TopRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(29)
                .done()
            .done()
        .profile(RightProfileId)
            .axisPlacement(vec3f(0, -1, 0).normalize(), topRight)
            .moveInitializer(X_AXISf * -1.0f)
            .scheduledBy(RightRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(19)
                .done()
            .done()
        .profile(BottomProfileId)
            .axisPlacement(vec3f(1, 0, 0).normalize(), bottomLeft)
            .moveInitializer(Y_AXISf)
            .scheduledBy(BottomRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(29)
                .done()
            .done()
        .commitCyclic<4>();
```

### Example: Separate Profile and Scheduling Configuration

When the same profiles need different scheduling strategies (e.g., per-level difficulty), use `commitProfilesOnly()` to register pool and profiles without schedulers, then attach rules separately via `SchedulerBuilder`:

```cpp
using namespace helios::engine::builder::spawnSystem;
using namespace helios::engine::builder::spawnSystem::builders::configs;

// 1. Register pool + profiles (no schedulers created)
SpawnSystemFactory::configure(poolManager, spawnManager)
    .pool(EdgeEnemyPoolId, edgePrefab, 100)
        .profile(LeftProfileId)
            .axisPlacement(vec3f(0, -1, 0).normalize(), topLeft)
            .moveInitializer(X_AXISf)
            .done()
        .profile(RightProfileId)
            .axisPlacement(vec3f(0, -1, 0).normalize(), topRight)
            .moveInitializer(X_AXISf * -1.0f)
            .done()
        .commitProfilesOnly();

// 2. Attach scheduling independently — can vary by level/difficulty
builders::SchedulerBuilder sb(spawnManager);
sb.cyclicScheduler(
    SchedulerConfig(LeftProfileId, LeftRuleId)
        .timerWithAvailabilityCondition(15.0f)
        .fixedAmount(19),
    SchedulerConfig(RightProfileId, RightRuleId)
        .timerWithAvailabilityCondition(15.0f)
        .fixedAmount(19)
);
```

This pattern is useful when:
- The same entity types are shared across multiple levels
- Scheduling parameters change per difficulty or game mode
- Profile configuration (placement, initialization) remains constant

## Complete Setup (Manual)

For cases where the builder is not suitable, the spawn system can be configured manually:

```cpp
// 1. Register managers
auto& poolManager  = gameWorld.addManager<GameObjectPoolManager>();
auto& spawnManager = gameWorld.addManager<SpawnManager>();

// 2. Configure pool
poolManager.addPoolConfig(std::make_unique<GameObjectPoolConfig>(
    enemyPoolId, enemyPrefab, 50
));

// 3. Create and register profile
auto profile = std::make_unique<SpawnProfile>(SpawnProfile{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer>(),
    .spawnInitializer = std::make_unique<MoveInitializer>(DirectionType::Random)
});
spawnManager.addSpawnProfile(enemyProfileId, std::move(profile));

// 4. Create scheduler with rules
auto scheduler = std::make_unique<DefaultSpawnScheduler>();
scheduler->addRule(enemyProfileId, std::make_unique<SpawnRule>(
    std::make_unique<TimerSpawnCondition>(3.0f),
    std::make_unique<FixedSpawnAmount>(2),
    enemyRuleId
));
spawnManager.addScheduler(std::move(scheduler));

// 5. Register dispatchers (always required)
commandBuffer.addDispatcher<ScheduledSpawnPlanCommandDispatcher>();
commandBuffer.addDispatcher<SpawnCommandDispatcher>();
commandBuffer.addDispatcher<DespawnCommandDispatcher>();

// 6. Add spawn system to game loop
gameLoop.phase(PhaseType::Pre)
    .addPass<GameState>(GameState::Running)
    .addSystem<GameObjectSpawnSystem>(spawnManager);
```

## Projectile Spawning

For projectiles (bullets, missiles), use the `EmitterContext` to spawn relative to the firing entity:

```cpp
// In ShootComponent or similar
void fire(const UpdateContext& ctx, const vec3f& position, const vec3f& direction) {
    SpawnContext spawnCtx;
    spawnCtx.emitterContext = EmitterContext{
        .position = position,
        .direction = direction,
        .velocity = direction * bulletSpeed_
    };
    
    ctx.commandBuffer().add<SpawnCommand>(bulletProfileId, spawnCtx, 1);
}
```

The `EmitterSpawnPlacer` and `EmitterInitializer` then use this context:

```cpp
auto bulletProfile = std::make_unique<SpawnProfile>(SpawnProfile{
    .gameObjectPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer>(),
    .spawnInitializer = std::make_unique<EmitterInitializer>()
});
```

## Despawning

To return entities to their pool, use `DespawnCommand`:

```cpp
// When entity should be removed (e.g., out of bounds, destroyed)
ctx.commandBuffer().add<DespawnCommand>(entity->guid(), profileId);
```

The `LevelBoundsBehaviorComponent` with `BoundsBehavior::Despawn` handles this automatically for entities leaving the level bounds.

## Module Structure

```
helios.engine.runtime.spawn/
├── SpawnManager.ixx               # Manager processing commands
├── SpawnProfile.ixx               # Profile configuration
├── SpawnContext.ixx               # Context for spawn operations
├── EmitterContext.ixx             # Emitter state for projectiles
├── SpawnCommandHandler.ixx        # Interface for command handlers
├── SpawnCommandHandlerRegistry.ixx # Registry mapping profiles to handlers
├── behavior/
│   ├── SpawnPlacer.ixx            # Placement interface
│   ├── SpawnInitializer.ixx       # Initialization interface
│   ├── placements/                # Concrete placers
│   │   ├── RandomSpawnPlacer.ixx
│   │   ├── EmitterSpawnPlacer.ixx
│   │   ├── AxisSpawnPlacer.ixx
│   │   ├── ColumnSpawnPlacer.ixx
│   │   └── DistributedSpawnPlacer.ixx
│   └── initializers/              # Concrete initializers
│       ├── EmitterInitializer.ixx
│       ├── MoveInitializer.ixx
│       ├── RandomDirectionInitializer.ixx
│       ├── DelayedComponentEnablerInitializer.ixx
│       └── InitializerList.ixx
├── policy/
│   ├── SpawnCondition.ixx         # Condition interface
│   ├── SpawnConditionAll.ixx      # Composite condition (AND)
│   ├── SpawnRule.ixx              # Rule combining condition + amount
│   ├── amount/                    # Amount providers
│   │   ├── FixedSpawnAmount.ixx
│   │   ├── SpawnAll.ixx
│   │   └── SpawnAmountByCallback.ixx
│   └── conditions/                # Concrete conditions
│       ├── TimerSpawnCondition.ixx
│       └── RequestedAmountIsAvailableCondition.ixx
├── scheduling/
│   ├── SpawnScheduler.ixx         # Rule evaluation interface
│   ├── DefaultSpawnScheduler.ixx  # Independent rule evaluation
│   ├── CyclicSpawnScheduler.ixx   # Round-robin rule evaluation
│   ├── SpawnPlan.ixx              # Planned spawn data
│   └── ScheduledSpawnPlan.ixx     # Plan with profile ID
├── commands/                      # Spawn/Despawn commands
├── dispatcher/                    # Command dispatchers
└── events/                        # Frame events

helios.engine.builder.spawnSystem/
├── SpawnSystemFactory.ixx         # Fluent entry point
├── builders/
│   ├── SchedulerBuilder.ixx       # Standalone scheduler registration
│   └── configs/
│       ├── SpawnPoolConfig.ixx    # Pool → profile → rule hierarchy
│       └── SchedulerConfig.ixx    # Standalone rule config (for SchedulerBuilder)
```

## See Also

- [GameObject Builder](gameobject-builder.md) — Fluent builder for GameObjects (callback-based)
- [Object Pooling](object-pooling.md) — Pool lifecycle and configuration
- [Component System](component-system.md) — Components used by spawned entities
- [Game Loop Architecture](gameloop-architecture.md) — How spawning integrates with phases
- [Command System](command-system.md) — Command pipeline for spawn commands
