/**
 * @file registry.ixx
 * @brief Component registration for renderable module.
 */
module;

export module helios.engine.modules.rendering.renderable.registry;

import helios.ecs.ComponentReflector;
import helios.engine.modules.rendering.renderable.components;

export namespace helios::engine::modules::rendering::renderable {

    /**
     * @brief Registers all renderable components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;


        R::template registerType<components::RenderableComponent<typename TEntityManager::Handle_type>>();
    }

}

