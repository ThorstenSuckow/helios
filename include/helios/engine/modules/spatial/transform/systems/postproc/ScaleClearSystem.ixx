/**
 * @file ScaleClearSystem.ixx
 * @brief System for resetting dirty flags on ScaleComponents.
 */
module;

#include <helios/engine/ecs/query/GameObjectView.h>

export module helios.engine.modules.spatial.transform.systems.postproc.ScaleClearSystem;

import helios.engine.runtime.world.GameWorld;
import helios.engine.ecs.System;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.modules.spatial.transform.components.ScaleComponent;


export namespace helios::engine::modules::spatial::transform::systems::postproc {

    /**
     * @brief System that clears the dirty state of ScaleComponents at the end of a frame.
     *
     * @details This system runs in the post-update phase. It iterates over all
     * ScaleComponents and resets their dirty flag if it was set. This ensures that
     * changes are only processed once per frame by other systems (like SceneSyncSystem).
     */
    class ScaleClearSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Updates the system, clearing dirty flags.
         *
         * @param updateContext The update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, sc] : gameWorld_->find<helios::engine::modules::spatial::transform::components::ScaleComponent>().each()) {
                sc->clearDirty();
            }

        }

    };


}