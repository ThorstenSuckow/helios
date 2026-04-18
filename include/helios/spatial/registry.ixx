/**
 * @file registry.ixx
 * @brief Component registration for entire spatial module.
 */
module;

export module helios.spatial.registry;

import helios.spatial.transform.registry;

export namespace helios::spatial {

    /**
     * @brief Registers all spatial components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::spatial::transform::registerComponents<TEntityManager>();
    }

}

