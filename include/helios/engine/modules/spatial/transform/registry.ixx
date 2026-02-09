/**
 * @file registry.ixx
 * @brief Component registration for spatial transform module.
 */
module;

export module helios.engine.modules.spatial.transform.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.spatial.transform.components;

export namespace helios::engine::modules::spatial::transform {

    /**
     * @brief Registers all spatial transform components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::ComposeTransformComponent>();
        R::registerType<components::TranslationStateComponent>();
        R::registerType<components::ScaleStateComponent>();
        R::registerType<components::RotationStateComponent>();
    }

}

