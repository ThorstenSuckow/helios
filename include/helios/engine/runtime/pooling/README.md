# helios::engine::runtime::pooling

GameObject pooling and request handling for efficient object recycling.

## Overview

This module provides infrastructure for memory-efficient entity management through object pooling. Pools pre-allocate GameObjects that can be acquired and released during gameplay, avoiding repeated allocation overhead.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObjectPool` | Container holding pre-allocated GameObject instances |
| `GameObjectPoolRegistry` | Central registry mapping pool IDs to pools |
| `GameObjectPoolFacade` | Simplified access to pool operations via GameWorld |
| `PoolRequestHandler` | Abstract base for processing pool acquire/release requests |
| `PoolRequestHandlerRegistry` | Registry for pool request handlers |



---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::pooling
@brief GameObject pooling and request handling for efficient object recycling.
@details Provides GameObjectPool for pre-allocation, pooling registries, and request handlers for deferred acquire/release operations.
</p></details>

