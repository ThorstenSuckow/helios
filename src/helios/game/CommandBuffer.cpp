module;

#include <format>
#include <deque>
#include <memory>

module helios.game.CommandBuffer;

import helios.game.Command;
import helios.game.GameWorld;
import helios.game.GameObject;
import helios.util.Guid;
import helios.util.log.Logger;
import helios.util.log.LogManager;

namespace helios::game {

    void CommandBuffer::add(const helios::util::Guid& guid, std::unique_ptr<helios::game::Command> command) {
        if (!command) {
            logger_.warn("Attempted to add a null command to the command buffer");
            return;
        }
        commandBuffer_.emplace_back(TargetedCommand{guid, std::move(command)});
    }

    CommandBuffer& CommandBuffer::flush(helios::game::GameWorld& gameWorld) {
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

} // namespace helios::game

