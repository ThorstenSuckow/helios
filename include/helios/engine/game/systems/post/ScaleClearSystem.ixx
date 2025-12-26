/**
 * @file ScaleClearSystem.ixx
 * @brief System for resetting dirty flags on ScaleComponents.
 */
module;

export module helios.engine.game.systems.post.ScaleClearSystem;

import helios.engine.game.GameWorld;
import helios.engine.game.System;
import helios.engine.game.UpdateContext;

import helios.engine.game.components.physics.ScaleComponent;


export namespace helios::engine::game::systems::post {

    /**
     * @brief System that clears the dirty state of ScaleComponents at the end of a frame.
     *
     * @details This system runs in the post-update phase. It iterates over all
     * ScaleComponents and resets their dirty flag if it was set. This ensures that
     * changes are only processed once per frame by other systems (like SceneSyncSystem).
     */
    class ScaleClearSystem : public System {


    public:


        /**
         * @brief Updates the system, clearing dirty flags.
         *
         * @param updateContext The update context.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            gameWorld_->find<helios::engine::game::components::physics::ScaleComponent>().each(
                [](auto* entity, auto& sc) {

                if (sc.isDirty()) {
                    sc.clearDirty();
                }

            });

        }

    };


}