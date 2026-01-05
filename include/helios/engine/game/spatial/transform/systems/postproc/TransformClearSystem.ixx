/**
 * @file TransformClearSystem.ixx
 * @brief System for resetting dirty flags on TransformComponents.
 */
module;

#include <helios/engine/game/GameObjectView.h>

export module helios.engine.game.spatial.transform.systems.postproc.TransformClearSystem;

import helios.engine.game.GameWorld;
import helios.engine.game.System;
import helios.engine.game.UpdateContext;

import helios.engine.game.components.scene.SceneNodeComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;


export namespace helios::engine::game::spatial::transform::systems::postproc {

    /**
     * @brief System that clears the dirty state of TransformComponents at the end of a frame.
     *
     * @details This system runs in the post-update phase. It iterates over all
     * TransformComponents and resets their dirty flag if it was set. This ensures that
     * changes are only processed once per frame by other systems (like SceneSyncSystem).
     */
    class TransformClearSystem : public System {


    public:

        /**
         * @brief Updates the system, clearing dirty flags.
         *
         * @param updateContext The update context.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            for (auto [entity, tc] : gameWorld_->find<
                helios::engine::game::spatial::transform::components::TransformComponent
                >().each()) {

                tc->clearDirty();
            }


        }

    };

}