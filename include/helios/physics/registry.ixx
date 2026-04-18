/**
 * @file registry.ixx
 * @brief Component registration for entire physics module.
 */
module;

export module helios.physics.registry;

import helios.physics.collision.registry;
import helios.physics.motion.registry;

export namespace helios::physics {

    /**
     * @brief Registers all physics components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::physics::collision::registerComponents<TEntityManager>();
        helios::physics::motion::registerComponents<TEntityManager>();
    }

}

