/**
 * @file registry.ixx
 * @brief Component registration for all engine modules.
 */
module;

export module helios.engine.modules.registry;

import helios.engine.modules.ai.registry;
import helios.engine.modules.rendering.registry;
import helios.engine.modules.spatial.registry;
import helios.engine.modules.ui.registry;
import helios.engine.modules.scene.registry;
import helios.engine.modules.physics.registry;
import helios.engine.modules.effects.registry;

export namespace helios::engine::modules {

    /**
     * @brief Registers all module components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::engine::modules::ai::registerComponents<TEntityManager>();
        helios::engine::modules::rendering::registerComponents<TEntityManager>();
        helios::engine::modules::spatial::registerComponents<TEntityManager>();
        helios::engine::modules::ui::registerComponents<TEntityManager>();
        helios::engine::modules::scene::registerComponents<TEntityManager>();
        helios::engine::modules::physics::registerComponents<TEntityManager>();
        helios::engine::modules::effects::registerComponents<TEntityManager>();
    }

}

