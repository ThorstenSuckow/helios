# helios::engine::core::data

Data structures for efficient entity management and querying.

This module provides core data structures used for managing GameObjects within the helios engine.

**Components:**

- **ComponentTypeId** - Compile-time type identifier for O(1) component indexing within GameObject.
- **GameObjectPool** - Low-level O(1) object pooling for entity recycling.
- **GameObjectPoolId** - Strongly-typed identifier for referencing pools.
- **GameObjectPoolRegistry** - Central registry managing multiple pools by ID.
- **GameObjectFilter** - Bitmask enum for filtering entities by active/inactive state and component state.
- **GameObjectView** - Lazy range adapter for component-filtered iteration with structured binding support.
- **SpawnProfileId** - Strongly-typed identifier for spawn profiles.
- **SpawnRuleId** - Strongly-typed identifier for spawn rules.

> **Note:** `GameObjectView.h` is intentionally a .h header instead of a .ixx module interface due to MSVC ICE issues with structured bindings.

> **Tip:** Use `GameObjectPoolFacade` (in `helios.engine.facade`) to coordinate pool operations with the GameWorld. The facade validates entities, handles stale pool entries, and deactivates released objects automatically.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::core::data
@brief Data structures for efficient entity management.
@details This namespace provides core data structures for managing GameObjects, including object pools for memory-efficient entity recycling (GameObjectPool, GameObjectPoolId, GameObjectPoolRegistry), filtered views for querying entities based on state or components (GameObjectFilter, GameObjectView). For coordinated pool/world operations, see GameObjectPoolFacade.
</p></details>
