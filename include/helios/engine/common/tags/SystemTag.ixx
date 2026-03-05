/**
 * @file SystemTag.ixx
 * @brief Tag type for identifying System-role classes at compile time.
 */
module;

export module helios.engine.common.tags.SystemTag;

export namespace helios::engine::common::tags {

    /**
     * @brief Compile-time tag identifying a class as a System.
     *
     * @details Declare `using EngineRoleTag = SystemTag;` inside a class
     * to make it satisfy the `IsSystem` concept. This enables registration
     * via `SystemRegistry::add<T>()`.
     *
     * @see IsSystem
     * @see HasTag
     */
    struct SystemTag{};
}