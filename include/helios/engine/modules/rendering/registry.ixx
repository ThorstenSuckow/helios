/**
 * @file registry.ixx
 * @brief Component registration for entire rendering module.
 */
module;

export module helios.engine.modules.rendering.registry;

import helios.engine.modules.rendering.renderable.registry;
import helios.engine.modules.rendering.model.registry;

export namespace helios::engine::modules::rendering {

    /**
     * @brief Registers all rendering components with the ComponentReflector.
     */
    inline void registerComponents() {
        helios::engine::modules::rendering::renderable::registerComponents();
        helios::engine::modules::rendering::model::registerComponents();
    }

}

