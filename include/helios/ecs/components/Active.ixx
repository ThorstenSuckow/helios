/**
 * @file Active.ixx
 * @brief Tag component indicating an active entity.
 */
module;

export module helios.ecs.components.Active;

export namespace helios::ecs::components {

    /**
     * @brief Tag component indicating that an entity is active.
     *
     * @details This empty struct serves as a marker component for filtering
     * active entities in views. It is automatically managed by `Entity<TEntityManager>::setActive()`.
     *
     *
     * @tparam THandle The entity handle type used for domain-specific typing.
     *
     * @see Inactive
     * @see Entity::setActive
     */
    template<typename THandle>
    struct Active {};

}