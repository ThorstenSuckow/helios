module;

#include <format>
#include <deque>
#include <memory>

module helios.engine.game.CommandBuffer;

import helios.engine.game.Command;
import helios.engine.game.GameWorld;
import helios.engine.game.GameObject;
import helios.util.Guid;
import helios.util.log.Logger;
import helios.util.log.LogManager;

namespace helios::engine::game {

    void CommandBuffer::add(const helios::util::Guid& guid, std::unique_ptr<helios::engine::game::Command> command) {
        if (!command) {
            logger_.warn("Attempted to add a null command to the command buffer");
            return;
        }
        commandBuffer_.emplace_back(TargetedCommand{guid, std::move(command)});
    }

    CommandBuffer& CommandBuffer::flush(helios::engine::game::GameWorld& gameWorld) {
        for (auto& targetedCommand : commandBuffer_) {
            auto* gameObject = gameWorld.find(targetedCommand.guid);
            if (!gameObject) {
                logger_.warn(std::format("GameObject with Guid {} not found, skipping command",
                                         targetedCommand.guid.value()));
                continue;
            }
            targetedCommand.command->execute(*gameObject);
        }
        clear();
        return *this;
    }

    CommandBuffer& CommandBuffer::clear() {
        commandBuffer_.clear();
        return *this;
    }

} // namespace helios::engine::game

