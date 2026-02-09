/**
 * @file Active.ixx
 * @brief Tag component indicating an active entity.
 */
module;

export module helios.engine.mechanics.lifecycle.components.Active;

export namespace helios::engine::mechanics::lifecycle::components {

    /**
     * @brief Tag component indicating that an entity is active.
     *
     * @details This empty struct serves as a marker component for filtering
     * active entities in views. It is automatically managed by `GameObject::setActive()`.
     *
     * When `setActive(true)` is called:
     * - The `Active` tag is added
     * - The `Inactive` tag is removed
     * - `onActivate()` is called on components that support it
     *
     * ## Usage in Views
     *
     * ```cpp
     * // Query only active entities
     * for (auto [entity, health, active] : gameWorld->view<
     *     HealthComponent,
     *     Active
     * >().whereEnabled()) {
     *     // Process only active entities
     * }
     * ```
     *
     * @see Inactive
     * @see GameObject::setActive
     */
    struct Active {};

}