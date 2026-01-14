# helios::engine::factory

Factory classes for creating and pooling GameObjects from prefab templates.

This module provides factory patterns for efficient GameObject creation. The primary class, `GameObjectFactory`, holds a prototype (prefab) and can mass-produce clones into a `GameObjectPool`, enabling efficient spawning by pre-allocating objects and reusing them via pooling.


---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::factory
@brief Factory classes for efficient GameObject creation.
@details This module provides factory patterns for creating and pooling GameObjects. The `GameObjectFactory` class holds a prototype (prefab) and can fill a `GameObjectPool` with clones, enabling memory-efficient spawning and entity reuse.
</p></details>


