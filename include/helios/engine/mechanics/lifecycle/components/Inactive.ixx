/**
 * @file Inactive.ixx
 * @brief Tag component indicating an inactive entity.
 */
module;

export module helios.engine.mechanics.lifecycle.components.Inactive;

export namespace helios::engine::mechanics::lifecycle::components {

    /**
     * @brief Tag component indicating that an entity is inactive.
     *
     * @details This empty struct serves as a marker component for filtering
     * inactive entities in views. It is automatically managed by `GameObject::setActive()`.
     *
     * When `setActive(false)` is called:
     * - The `Inactive` tag is added
     * - The `Active` tag is removed
     * - `onDeactivate()` is called on components that support it
     *
     * ## Usage in Views
     *
     * ```cpp
     * // Exclude inactive entities
     * for (auto [entity, health] : gameWorld->view<HealthComponent>()
     *     .exclude<Inactive>()) {
     *     // Process only non-inactive entities
     * }
     * ```
     *
     * @see Active
     * @see GameObject::setActive
     */
    struct Inactive {};

}