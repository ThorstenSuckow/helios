/**
 * @file registry.ixx
 * @brief Component registration for entire effects module.
 */
module;

export module helios.engine.modules.effects.registry;

import helios.engine.modules.effects.gfx.registry;

export namespace helios::engine::modules::effects {

    /**
     * @brief Registers all effects components with the ComponentReflector.
     */
    inline void registerComponents() {
        helios::engine::modules::effects::gfx::registerComponents();
    }

}

