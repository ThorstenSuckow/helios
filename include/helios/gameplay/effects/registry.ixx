/**
 * @file registry.ixx
 * @brief Component registration for entire effects module.
 */
module;

export module helios.gameplay.effects.registry;

import helios.gameplay.effects.gfx.registry;

export namespace helios::gameplay::effects {

    /**
     * @brief Registers all effects components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::gameplay::effects::gfx::registerComponents<TEntityManager>();
    }

}

