/**
 * @file registry.ixx
 * @brief Component registration for scene module.
 */
module;

export module helios.engine.modules.scene.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.scene.components;

export namespace helios::engine::modules::scene {

    /**
     * @brief Registers all scene components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::SceneNodeComponent>();
    }

}

