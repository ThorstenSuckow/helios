module;

export module helios.examples.ecs_rendering.GameLoop;

import helios.engine.runtime.gameloop;
import helios.engine.mechanics.gamestate.types.GameState;
import helios.engine.mechanics.gamestate.systems;
import helios.engine.mechanics.lifecycle.systems;
import helios.engine.modules.spatial.transform.systems;
import helios.engine.common.types;
import helios.platform.environment.systems;
import helios.platform.environment.types;
import helios.platform.window.systems;
import helios.platform.window.types;
import helios.platform.glfw.systems;
import helios.engine.runtime.world.types;

import helios.engine.state.Bindings;

using namespace helios::engine::runtime::gameloop;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::gamestate::systems;
using namespace helios::engine::mechanics::lifecycle::systems;
using namespace helios::engine::modules::spatial::transform::systems;
using namespace helios::engine::common::types;
using namespace helios::platform::environment::systems;
using namespace helios::platform::environment::types;
using namespace helios::engine::runtime::world::types;
using namespace helios::platform::window::systems;
using namespace helios::platform::window::types;
using namespace helios::platform::glfw::systems;
export namespace helios::examples::ecs_rendering {

    class GameLoopConfig {

    public:
        static void configure(helios::engine::runtime::gameloop::GameLoop& gameLoop) {


        }
    };


}