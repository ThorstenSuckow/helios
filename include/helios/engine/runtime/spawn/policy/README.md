# helios::engine::runtime::spawn::policy

Spawn rules, conditions, and amount providers.

## Overview

This module defines the *policy* layer of runtime spawning.
`SpawnRule<THandle>` combines:

- a `SpawnCondition` (*when to spawn*)
- a `SpawnAmountProvider<THandle>` (*how many to spawn*)

Rule execution state is tracked via `SpawnRuleState`.

> **Note:** `SpawnCondition` is *not* template-based — conditions operate on
> pool snapshots and rule state independent of the handle type.

## Key Types

| Type | Purpose |
|------|---------|
| `SpawnRule<THandle>` | Composes condition and amount provider into one evaluatable rule |
| `SpawnCondition` | Interface for spawn trigger checks |
| `SpawnConditionAll` | Composite AND-condition over multiple `SpawnCondition` instances |
| `SpawnAmountProvider<THandle>` | Interface for dynamic amount calculation |
| `SpawnRuleState` | Mutable per-rule runtime state |

## Submodules

| Directory | Purpose |
|-----------|---------|
| `amount/` | Amount provider implementations |
| `conditions/` | Condition implementations |

## Evaluation Flow

1. Scheduler selects active rules.
2. Rule processor updates `SpawnRuleState`.
3. `SpawnCondition` decides whether spawning is allowed.
4. `SpawnAmountProvider<THandle>` computes spawn count.
5. Result is encoded in `SpawnPlan`.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::policy
@brief Spawn rules, conditions, and amount providers.
@details Template-based policy primitives used by spawn schedulers.
</p></details>
