/**
 * @file registry.ixx
 * @brief Component registration for physics motion module.
 */
module;

export module helios.physics.motion.registry;

import helios.ecs.ComponentReflector;
import helios.physics.motion.components;

export namespace helios::physics::motion {

    /**
     * @brief Registers all motion components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::DirectionComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::Move2DComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::SteeringComponent<typename TEntityManager::Handle_type>>();
    }

}

