/**
 * @file registry.ixx
 * @brief Component registration for all engine mechanics.
 */
module;

export module helios.gameplay.registry;

import helios.gameplay.scoring.registry;
import helios.gameplay.health.registry;
import helios.gameplay.spawn.registry;
import helios.gameplay.bounds.registry;
import helios.gameplay.combat.registry;
import helios.gameplay.damage.registry;
import helios.gameplay.lifecycle.registry;

export namespace helios::gameplay {

    /**
     * @brief Registers all mechanics components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::gameplay::scoring::registerComponents<TEntityManager>();
        helios::gameplay::health::registerComponents<TEntityManager>();
        helios::gameplay::spawn::registerComponents<TEntityManager>();
        helios::gameplay::bounds::registerComponents<TEntityManager>();
        helios::gameplay::combat::registerComponents<TEntityManager>();
        helios::gameplay::damage::registerComponents<TEntityManager>();
        helios::gameplay::lifecycle::registerComponents<TEntityManager>();
    }

}

