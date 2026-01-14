
module;

#include <cassert>
#include <memory>

export module helios.engine.mechanics.spawn.dispatcher.DespawnCommandDispatcher;

import helios.engine.mechanics.spawn.commands.DespawnCommand;

import helios.engine.game.pool.components.PoolIdComponent;

import helios.engine.mechanics.spawn.requests.DespawnRequest;
          

import helios.engine.core.messaging.command.WorldCommandDispatcher;
import helios.engine.core.messaging.command.TypedWorldCommandDispatcher;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.pooling.PoolRequestHandler;





export namespace helios::engine::mechanics::spawn::dispatcher {

    class DespawnCommandDispatcher final : public helios::engine::core::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::mechanics::spawn::commands::DespawnCommand> {


    protected:
        void dispatchTyped(
        helios::engine::runtime::world::GameWorld& gameWorld,
        const helios::engine::mechanics::spawn::commands::DespawnCommand& command
        ) noexcept override {

            auto guid = command.guid();
            auto pic = command.gameObjectPoolId();

            if (!pic) {
                assert(false && "Missing support for despawns w/o pool ownership");
                return;
            }

            auto* poolRequestHandler = gameWorld.poolRequestHandler(pic.value());

            if (poolRequestHandler) {
                helios::engine::mechanics::spawn::requests::DespawnRequest despawnRequest(
                    guid, pic
                );

                poolRequestHandler->submit(despawnRequest);
            }
        };

    public:

        DespawnCommandDispatcher() = default;



    };


}