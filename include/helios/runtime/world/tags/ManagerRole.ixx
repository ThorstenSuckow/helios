/**
 * @file ManagerRole.ixx
 * @brief Tag type for identifying Manager-role classes at compile time.
 */
module;

export module helios.runtime.world.tags.ManagerRole;

export namespace helios::runtime::tags {

    /**
     * @brief Compile-time tag identifying a class as a Manager.
     *
     * @details Declare `using EngineRoleTag = ManagerRole;` inside a class
     * to make it satisfy the `IsManagerLike` concept. This enables registration
     * via `GameWorld::registerResource<T>()`.
     *
     * @see IsManagerLike
     * @see HasTag
     */
    struct ManagerRole{};
}