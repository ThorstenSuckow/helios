/**
 * @file registry.ixx
 * @brief Component registration for entire rendering module.
 */
module;

export module helios.rendering.registry;

import helios.ecs.ComponentReflector;


import helios.rendering.model.registry;

import helios.rendering.components;

export namespace helios::rendering {

    /**
     * @brief Registers all rendering components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::rendering::model::registerComponents<TEntityManager>();

        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::RenderPrototypeComponent<typename TEntityManager::Handle_type>>();
    }

}

