module;

#include <type_traits>
#include <utility>

module helios.engine.gameloop.Pass;

import helios.engine.game.System;
import helios.engine.game.SystemRegistry;

import helios.engine.game.UpdateContext;
import helios.engine.game.GameWorld;

import helios.engine.gameloop.GameLoop;
import helios.engine.gameloop.Phase;

namespace helios::engine::gameloop {


    void Pass::init(helios::engine::game::GameWorld& gameWorld) {
        for (auto& sys : systemRegistry_.systems()) {
            sys->init(gameWorld);
        }
    }
    void Pass::update(helios::engine::game::UpdateContext& updateContext) {
        for (auto& sys : systemRegistry_.systems()) {
            sys->update(updateContext);
        }
    }

    void Pass::commit() {
        if (hasCommitPoint_) {
            owner_.gameLoop().passCommit();
        }
    }


}