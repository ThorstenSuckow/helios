/**
 * @file registry.ixx
 * @brief Component registration for renderable module.
 */
module;

export module helios.engine.modules.rendering.renderable.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.rendering.renderable.components;

export namespace helios::engine::modules::rendering::renderable {

    /**
     * @brief Registers all renderable components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::RenderableComponent>();
    }

}

