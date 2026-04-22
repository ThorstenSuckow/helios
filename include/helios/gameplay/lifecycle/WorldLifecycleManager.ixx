/**
 * @file WorldLifecycleManager.ixx
 * @brief Manager for deferred world-level lifecycle operations.
 */
module;

#include <vector>
#include <cassert>

export module helios.gameplay.lifecycle.WorldLifecycleManager;

import helios.runtime.messaging.command.CommandHandlerRegistry;
import helios.runtime.world.UpdateContext;

import helios.gameplay.lifecycle.types;
import helios.gameplay.lifecycle.commands.WorldLifecycleCommand;
import helios.runtime.world.tags.ManagerRole;

using namespace helios::runtime::world;
using namespace helios::gameplay::lifecycle::commands;
using namespace helios::gameplay::lifecycle::types;
using namespace helios::runtime::messaging::command;
export namespace helios::gameplay::lifecycle {

    /**
     * @brief Manager that processes deferred world lifecycle commands.
     *
     * @details WorldLifecycleManager collects WorldLifecycleCommands via
     * submit() and executes them during flush(). This allows systems to
     * request world-level operations (e.g. reset) through the command
     * pipeline instead of calling GameWorld methods directly.
     *
     * On Reset, the pending queue is moved to a local variable before
     * processing. This prevents re-entrant issues since GameWorld::reset()
     * calls Manager::reset() on all managers, including this one.
     *
     * @see WorldLifecycleCommand
     * @see WorldLifecycleAction
     * @see GameWorld::reset
     */
    class WorldLifecycleManager {

        /**
         * @brief Pending commands queued for the next flush.
         */
        std::vector<WorldLifecycleCommand> pending_;


    public:

        using EngineRoleTag = helios::runtime::tags::ManagerRole;

        /**
         * @brief Enqueues a lifecycle command for deferred processing.
         *
         * @param cmd The command to enqueue.
         *
         * @return True if the command was accepted.
         */
        bool submit(WorldLifecycleCommand cmd) noexcept {
            pending_.push_back(cmd);
            return true;
        }

        /**
         * @brief Registers this manager as the WorldLifecycleCommand handler.
         *
         * @param gameWorld The GameWorld to register with.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry)  {
            commandHandlerRegistry.registerHandler<WorldLifecycleCommand>(*this);
        }

        /**
         * @brief Processes all pending lifecycle commands.
         *
         * @details Moves the pending queue to a local variable before
         * iterating. This is necessary because a Reset action triggers
         * GameWorld::reset(), which in turn calls reset() on this manager,
         * clearing pending_.
         *
         * @param updateContext The current frame's update context.
         */
        void flush(UpdateContext& updateContext) noexcept  {
            if (pending_.empty()) {
                return;
            }

            std::vector<WorldLifecycleCommand> queue = std::move(pending_);
            pending_.clear();

            for (const auto& cmd : queue) {
                switch (cmd.action()) {
                    case WorldLifecycleAction::Reset:
                        /**
                         * @todo make GameWorkd CommandHandler for WorldLifecycleAction::Reset
                         *
                         */
                        assert(false && "Needs to call GameWorld reset()");
                        // reset complete, do not process further commands.
                        return;
                }
            }

        }

        /**
         * @brief Clears all pending commands.
         */
        void reset()  {
            pending_.clear();
        }
    };

}

