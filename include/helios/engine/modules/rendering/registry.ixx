/**
 * @file registry.ixx
 * @brief Component registration for entire rendering module.
 */
module;

export module helios.engine.modules.rendering.registry;

import helios.ecs.ComponentReflector;

import helios.engine.modules.rendering.renderable.registry;
import helios.engine.modules.rendering.model.registry;

import helios.engine.modules.rendering.components;

export namespace helios::engine::modules::rendering {

    /**
     * @brief Registers all rendering components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::engine::modules::rendering::renderable::registerComponents<TEntityManager>();
        helios::engine::modules::rendering::model::registerComponents<TEntityManager>();

        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::RenderPrototypeComponent<typename TEntityManager::Handle_type>>();
    }

}

