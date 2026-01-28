/**
 * @file types.ixx
 * @brief Core type definitions and tag types for the helios engine.
 */
module;


export module helios.core.types;


export namespace helios::core::types {

    /**
     * @brief Tag type used to indicate skipping default initialization.
     *
     * @details `no_init_t` is a tag type that signals to constructors or
     * factory functions that default initialization should be skipped.
     * This is useful for performance-critical code paths where objects
     * will be immediately overwritten or initialized manually.
     *
     * ## Usage
     *
     * Pass `no_init` as a constructor argument to request uninitialized storage:
     *
     * ```cpp
     * import helios.util.Guid;
     * import helios.core.types;
     *
     * // Generate a new unique Guid
     * auto id = helios::util::Guid::generate();
     *
     * // Declare uninitialized Guid for later assignment
     * helios::util::Guid deferredId{helios::core::types::no_init};
     *
     * // Assign later
     * deferredId = helios::util::Guid::generate();
     * ```
     *
     * @note Objects constructed with `no_init` are in an indeterminate state.
     * Reading from them before assignment is undefined behavior.
     *
     * @see no_init
     * @see helios::util::Guid
     */
    struct no_init_t{};

    /**
     * @brief Tag constant for requesting uninitialized construction.
     *
     * @details This is a convenience instance of `no_init_t` that can be
     * passed to constructors supporting uninitialized construction.
     *
     * ```cpp
     * helios::util::Guid id{helios::core::types::no_init};
     * ```
     *
     * @see no_init_t
     * @see helios::util::Guid
     */
    inline constexpr no_init_t no_init;

}