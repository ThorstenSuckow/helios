/**
 * @file registry.ixx
 * @brief Component registration for all engine mechanics.
 */
module;

export module helios.engine.mechanics.registry;

import helios.engine.mechanics.scoring.registry;
import helios.engine.mechanics.health.registry;
import helios.engine.mechanics.spawn.registry;
import helios.engine.mechanics.bounds.registry;
import helios.engine.mechanics.combat.registry;
import helios.engine.mechanics.damage.registry;
import helios.engine.mechanics.lifecycle.registry;

export namespace helios::engine::mechanics {

    /**
     * @brief Registers all mechanics components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::engine::mechanics::scoring::registerComponents<TEntityManager>();
        helios::engine::mechanics::health::registerComponents<TEntityManager>();
        helios::engine::mechanics::spawn::registerComponents<TEntityManager>();
        helios::engine::mechanics::bounds::registerComponents<TEntityManager>();
        helios::engine::mechanics::combat::registerComponents<TEntityManager>();
        helios::engine::mechanics::damage::registerComponents<TEntityManager>();
        helios::engine::mechanics::lifecycle::registerComponents<TEntityManager>();
    }

}

