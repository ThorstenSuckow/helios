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
    inline void registerComponents() {
        helios::engine::mechanics::scoring::registerComponents();
        helios::engine::mechanics::health::registerComponents();
        helios::engine::mechanics::spawn::registerComponents();
        helios::engine::mechanics::bounds::registerComponents();
        helios::engine::mechanics::combat::registerComponents();
        helios::engine::mechanics::damage::registerComponents();
        helios::engine::mechanics::lifecycle::registerComponents();
    }

}

