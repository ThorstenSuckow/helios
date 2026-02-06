# helios::engine::mechanics::combat::components

Combat-specific components for aiming, shooting, and attack tracking.

These components implement high-level combat behaviors that coordinate between input, physics, and rendering systems.

## Components

| Component | Description |
|-----------|-------------|
| `Aim2DComponent` | Stores 2D aim direction and fire frequency |
| `ShootComponent` | Manages projectile shooting with cooldown timer |
| `LastAttackerComponent` | Tracks the last entity that attacked this entity |

## LastAttackerComponent

Stores an `AttackContext` containing:
- Source entity handle (the attacker)
- Contact point (where the attack landed)
- Damage applied

Used by `HealthUpdateSystem` to include attack context in `DeathEvent` for kill attribution and scoring.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::combat::components
@brief Combat-specific components for aiming, shooting, and attack tracking.
@details This namespace contains components that implement combat behaviors such as aiming direction tracking, projectile shooting with rate limiting, and attack attribution.
</p></details>

