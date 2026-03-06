/**
 * @file SystemRole.ixx
 * @brief Tag type for identifying System-role classes at compile time.
 */
module;

export module helios.engine.common.tags.SystemRole;

export namespace helios::engine::common::tags {

    /**
     * @brief Compile-time tag identifying a class as a System.
     *
     * @details Declare `using EngineRoleTag = SystemRole;` inside a class
     * to make it satisfy the `IsSystemLike` concept. This enables registration
     * via `SystemRegistry::add<T>()`.
     *
     * @see IsSystemLike
     * @see HasTag
     */
    struct SystemRole{};
}