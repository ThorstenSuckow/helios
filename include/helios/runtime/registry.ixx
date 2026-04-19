/**
 * @file registry.ixx
 * @brief Component registration for entire runtime module.
 */
module;

export module helios.runtime.registry;

import helios.runtime.pooling.registry;

export namespace helios::runtime {

    /**
     * @brief Registers all runtime components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::runtime::pooling::registerComponents<TEntityManager>();
    }

}


