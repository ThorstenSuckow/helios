/**
 * @file registry.ixx
 * @brief Component registration for entire physics module.
 */
module;

export module helios.engine.modules.physics.registry;

import helios.engine.modules.physics.collision.registry;
import helios.engine.modules.physics.motion.registry;

export namespace helios::engine::modules::physics {

    /**
     * @brief Registers all physics components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::engine::modules::physics::collision::registerComponents<TEntityManager>();
        helios::engine::modules::physics::motion::registerComponents<TEntityManager>();
    }

}

