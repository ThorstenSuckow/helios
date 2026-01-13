
module;

#include <cassert>
#include <memory>

export module helios.engine.game.gameplay.spawn.dispatcher.DespawnCommandDispatcher;

import helios.engine.game.gameplay.spawn.commands.DespawnCommand;

import helios.engine.game.pool.components.PoolIdComponent;

import helios.engine.game.gameplay.spawn.requests.DespawnRequest;
          

import helios.engine.core.messaging.command.WorldCommandDispatcher;
import helios.engine.core.messaging.command.TypedWorldCommandDispatcher;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.engine.game.PoolRequestHandler;





export namespace helios::engine::game::gameplay::spawn::dispatcher {

    class DespawnCommandDispatcher final : public helios::engine::core::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::game::gameplay::spawn::commands::DespawnCommand> {


    protected:
        void dispatchTyped(
        helios::engine::game::GameWorld& gameWorld,
        const helios::engine::game::gameplay::spawn::commands::DespawnCommand& command
        ) noexcept override {

            auto guid = command.guid();
            auto pic = command.gameObjectPoolId();

            if (!pic) {
                assert(false && "Missing support for despawns w/o pool ownership");
                return;
            }

            auto* poolRequestHandler = gameWorld.poolRequestHandler(pic.value());

            if (poolRequestHandler) {
                helios::engine::game::gameplay::spawn::requests::DespawnRequest despawnRequest(
                    guid, pic
                );

                poolRequestHandler->submit(despawnRequest);
            }
        };

    public:

        DespawnCommandDispatcher() = default;



    };


}