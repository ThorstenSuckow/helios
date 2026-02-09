/**
 * @file registry.ixx
 * @brief Component registration for entire spatial module.
 */
module;

export module helios.engine.modules.spatial.registry;

import helios.engine.modules.spatial.transform.registry;

export namespace helios::engine::modules::spatial {

    /**
     * @brief Registers all spatial components with the ComponentReflector.
     */
    inline void registerComponents() {
        helios::engine::modules::spatial::transform::registerComponents();
    }

}

