/**
 * @file registry.ixx
 * @brief Component registration for physics collision module.
 */
module;

export module helios.physics.collision.registry;

import helios.ecs.ComponentReflector;
import helios.physics.collision.components;

export namespace helios::physics::collision {

    /**
     * @brief Registers all collision components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::CollisionStateComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::CollisionComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::AabbColliderComponent<typename TEntityManager::Handle_type>>();
    }

}

