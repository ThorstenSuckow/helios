module;

#include <deque>
#include <memory>

export module helios.game.CommandBuffer;

import helios.game.Command;
import helios.game.GameWorld;
import helios.game.GameObject;
import helios.util.Guid;

struct TargetedCommand {
    helios::util::Guid guid;
    std::unique_ptr<helios::game::Command> command;
};

export namespace helios::game {



    class CommandBuffer {

        std::deque<TargetedCommand> commandBuffer_;

    public:


        void add(const helios::game::GameObject& target, std::unique_ptr<helios::game::Command> command) {

            if (!command) {
                /**
                 *@todo log
                 */
                return;
            }
            commandBuffer_.emplace_back(TargetedCommand{target.guid(), std::move(command)});

        }

        void add(const helios::util::Guid& guid, std::unique_ptr<helios::game::Command> command) {

            if (!command) {
                /**
                 *@todo log
                 */
                return;
            }
            commandBuffer_.emplace_back(TargetedCommand{guid, std::move(command)});

        }


        CommandBuffer& flush(helios::game::GameWorld& gameWorld) {
            for (auto& targetedCommand : commandBuffer_) {
                auto* gameObject = gameWorld.find(targetedCommand.guid);
                if (!gameObject) {
                    continue;
                }
                targetedCommand.command->execute(*gameObject);
            }
            clear();
            return *this;
        }

        CommandBuffer& clear() {
            commandBuffer_.clear();
            return *this;
        }


    };



}