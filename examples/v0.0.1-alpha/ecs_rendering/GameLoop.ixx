module;

export module helios.examples.ecs_rendering.GameLoop;

import helios.runtime.gameloop;
import helios.gameplay.gamestate.types.GameState;
import helios.gameplay.gamestate.systems;
import helios.gameplay.lifecycle.systems;
import helios.engine.modules.spatial.transform.systems;
import helios.engine.common.types;
import helios.platform.environment.systems;
import helios.platform.environment.types;
import helios.platform.window.systems;
import helios.platform.window.types;
import helios.platform.glfw.systems;
import helios.runtime.world.types;

import helios.state.Bindings;

using namespace helios::runtime::gameloop;
using namespace helios::gameplay::gamestate::types;
using namespace helios::gameplay::gamestate::systems;
using namespace helios::gameplay::lifecycle::systems;
using namespace helios::engine::modules::spatial::transform::systems;
using namespace helios::engine::common::types;
using namespace helios::platform::environment::systems;
using namespace helios::platform::environment::types;
using namespace helios::runtime::world::types;
using namespace helios::platform::window::systems;
using namespace helios::platform::window::types;
using namespace helios::platform::glfw::systems;
export namespace helios::examples::ecs_rendering {

    class GameLoopConfig {

    public:
        static void configure(helios::runtime::gameloop::GameLoop& gameLoop) {


        }
    };


}