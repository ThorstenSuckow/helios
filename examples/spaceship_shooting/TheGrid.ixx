/**
 * @file TheGrid.ixx
 * @brief Grid entity.
 */
module;


#include <format>

export module helios.examples.spaceshipShooting.TheGrid;

import helios.engine.game.GameObject;
import helios.math.types;
import helios.math.utils;
import helios.math.transform;
import helios.scene.SceneNode;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::examples::spaceshipShooting::TheGrid"
export namespace helios::examples::spaceshipShooting {


    class TheGrid : public helios::engine::game::GameObject {

        /**
         * @brief Logger instance for debug and diagnostic output.
         */
        static inline const helios::util::log::Logger& logger_ =
                helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);



    public:


        explicit TheGrid(helios::scene::SceneNode* sceneNode) : GameObject(sceneNode) {}

        void update(const float deltaTime) override {}

    };

}
