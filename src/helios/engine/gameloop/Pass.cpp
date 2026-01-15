module;

#include <type_traits>
#include <utility>

module helios.engine.runtime.gameloop.Pass;

import helios.engine.ecs.System;
import helios.engine.runtime.world.SystemRegistry;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.gameloop.GameLoop;
import helios.engine.runtime.gameloop.Phase;

namespace helios::engine::runtime::gameloop {


    void Pass::init(helios::engine::runtime::world::GameWorld& gameWorld) {
        for (auto& sys : systemRegistry_.systems()) {
            sys->init(gameWorld);
        }
    }
    void Pass::update(helios::engine::runtime::world::UpdateContext& updateContext) {
        for (auto& sys : systemRegistry_.systems()) {
            sys->update(updateContext);
        }
    }

    void Pass::commit(const CommitPoint commitPoint, helios::engine::runtime::world::UpdateContext& updateContext) {
        owner_.gameLoop().passCommit(commitPoint, updateContext);
    }


}