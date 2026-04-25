# helios::ecs::concepts

Compile-time constraints for ECS extension points.

## Overview

This submodule defines concepts and traits used to constrain template
parameters across `helios::ecs`. They validate extension points at compile
time (handle shape, lookup strategy API, lifecycle hooks).

## Concepts

| Concept | Purpose |
|---------|---------|
| `IsEntityHandle<T>` | Constrains types matching `EntityHandle<TDomainTag>` |
| `IsStrongIdCollisionResolverLike<T>` | Constrains registry lookup strategies (`add/remove/has`) |

> **Note:** Strong-id validation now lives in `helios::core::concepts::IsStrongIdLike`.

## Traits Namespace

`helios::ecs::concepts::traits` provides optional lifecycle hooks:

| Trait | Purpose |
|-------|---------|
| `HasOnAcquire<T>` | Pool acquisition hook (`onAcquire()`) |
| `HasOnRelease<T>` | Pool release hook (`onRelease()`) |
| `HasOnRemove<T>` | Removal interception hook (`onRemove() -> bool`) |
| `HasToggleable<T>` | Enable/disable hooks (`enable()`, `disable()`) |
| `HasClone<T>` | Post-copy hook (`onClone(const T&)`) |
| `HasActivatable<T>` | Activation hooks (`onActivate()`, `onDeactivate()`) |

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.ecs.EntityRegistry` | Consumes lookup strategy concepts |
| `helios.ecs.EntityHandle` | Primary handle shape validated by concepts |
| `helios.ecs.ComponentReflector` | Uses trait hooks for lifecycle registration |
| `helios.core.concepts` | Provides shared constraints such as `IsStrongIdLike` |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs::concepts
@brief Compile-time constraints for ECS extension points.
@details Defines concepts and traits that constrain generic ECS template parameters and lifecycle hooks.
</p></details>
