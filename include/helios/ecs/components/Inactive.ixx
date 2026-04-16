/**
 * @file Inactive.ixx
 * @brief Tag component indicating an inactive entity.
 */
module;

export module helios.ecs.components.Inactive;

export namespace helios::ecs::components {

    /**
     * @brief Tag component indicating that an entity is inactive.
     *
     * @details This empty struct serves as a marker component for filtering
     * inactive entities in views. It is automatically managed by `Entity<TEntityManager>::setActive()`.

     *
     * @tparam THandle The entity handle type used for domain-specific typing.
     *
     * @see Active
     * @see Entity::setActive
     */
    template<typename THandle>
    struct Inactive {};

}