module;

#include <type_traits>
#include <utility>

module helios.engine.gameloop.Pass;

import helios.engine.ecs.System;
import helios.engine.ecs.SystemRegistry;

import helios.engine.ecs.UpdateContext;
import helios.engine.ecs.GameWorld;

import helios.engine.gameloop.GameLoop;
import helios.engine.gameloop.Phase;

namespace helios::engine::gameloop {


    void Pass::init(helios::engine::ecs::GameWorld& gameWorld) {
        for (auto& sys : systemRegistry_.systems()) {
            sys->init(gameWorld);
        }
    }
    void Pass::update(helios::engine::ecs::UpdateContext& updateContext) {
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