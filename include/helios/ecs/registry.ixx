/**
 * @file registry.ixx
 * @brief Component registration for ecs module.
 */
module;

export module helios.ecs.registry;

import helios.ecs.ComponentReflector;
import helios.ecs.components;

using namespace helios::ecs::components;
export namespace helios::ecs {

    /**
     * @brief Registers all lifecycle components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = ComponentReflector<TEntityManager>;

        R::template registerType<HierarchyComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<Active<typename TEntityManager::Handle_type>>();
        R::template registerType<Inactive<typename TEntityManager::Handle_type>>();
    }

}

