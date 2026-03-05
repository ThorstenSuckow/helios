/**
 * @file ManagerTag.ixx
 * @brief Tag type for identifying Manager-role classes at compile time.
 */
module;

export module helios.engine.common.tags.ManagerTag;

export namespace helios::engine::common::tags {

    /**
     * @brief Compile-time tag identifying a class as a Manager.
     *
     * @details Declare `using EngineRoleTag = ManagerTag;` inside a class
     * to make it satisfy the `IsManager` concept. This enables registration
     * via `GameWorld::registerManager<T>()`.
     *
     * @see IsManager
     * @see HasTag
     */
    struct ManagerTag{};
}