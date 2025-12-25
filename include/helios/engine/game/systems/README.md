# helios::engine::game::systems

Global game systems that operate on the GameWorld rather than individual GameObjects.

Systems are ideal for cross-cutting concerns that span multiple entities, such as object pooling, physics simulation, or collision detection. They receive frame updates via the `Updatable` interface and have access to the full `GameWorld` context.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::systems
@brief Global game systems for cross-cutting concerns.
@details This namespace contains System implementations that operate on the GameWorld level. Systems handle concerns that span multiple GameObjects, such as object pooling (ProjectilePoolSystem), physics simulation, and collision detection.
</p></details>

