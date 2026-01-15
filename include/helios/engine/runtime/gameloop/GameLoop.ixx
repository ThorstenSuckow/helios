/**
 * @file GameLoop.ixx
 * @brief Central orchestrator for the game update cycle.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.runtime.gameloop.GameLoop;

import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.UpdateContext;
import helios.util.log.Logger;
import helios.util.log.LogManager;

import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.messaging.event.GameLoopEventBus;

import helios.engine.runtime.gameloop.CommitPoint;
import helios.engine.runtime.gameloop.Pass;
import helios.engine.runtime.gameloop.Phase;

import helios.input.InputSnapshot;

#define HELIOS_LOG_SCOPE "helios::engine::runtime::gameloop::GameLoop"
export namespace helios::engine::runtime::gameloop {

    /**
     * @brief Central orchestrator for the game update cycle.
     *
     * The GameLoop manages the execution of game systems across three distinct phases:
     * Pre, Main, and Post. Each phase can contain multiple passes, and each pass can
     * have a configurable commit point for fine-grained synchronization control.
     *
     * The GameLoop owns:
     * - Three phases (Pre, Main, Post), each containing passes with registered systems.
     * - A CommandBuffer for deferred command execution.
     * - Two event buses: one for phase-level and one for pass-level event propagation.
     *
     * Commit points allow systems to specify when commands should be flushed, managers
     * should process their requests, and pass-level events should be synchronized.
     * This enables deterministic ordering and fine-grained control over the update cycle.
     *
     * @see Phase
     * @see Pass
     * @see CommitPoint
     * @see CommandBuffer
     */
    class GameLoop {


        bool initialized_ = false;
    protected:


        friend class helios::engine::runtime::gameloop::Pass;

        /**
         * @brief The logger used with this GameLoop instance.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);


        /**
         * @brief The pre-update phase, executed before main gameplay logic.
         */
        helios::engine::runtime::gameloop::Phase prePhase_{*this};

        /**
         * @brief The main update phase for core gameplay systems.
         */
        helios::engine::runtime::gameloop::Phase mainPhase_{*this};

        /**
         * @brief The post-update phase for cleanup and synchronization.
         */
        helios::engine::runtime::gameloop::Phase postPhase_{*this};

        /**
         * @brief Buffer for deferred command execution.
         */
        helios::engine::runtime::messaging::command::CommandBuffer commandBuffer_{};

        /**
         * @brief Event bus for phase-level event propagation.
         *
         * Events pushed via `UpdateContext::pushPhase()` are buffered here
         * and become readable in the next phase via `UpdateContext::readPhase()`.
         * The buffer swap occurs in phaseCommit().
         *
         * @see UpdateContext::pushPhase()
         * @see UpdateContext::readPhase()
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus phaseEventBus_{};

        /**
         * @brief Event bus for pass-level event propagation.
         *
         * Events pushed via `UpdateContext::pushPass()` are buffered here
         * and become readable in subsequent passes via `UpdateContext::readPass()`.
         * The buffer swap occurs when a pass has a commit point.
         *
         * @see UpdateContext::pushPass()
         * @see UpdateContext::readPass()
         * @see Pass::addCommitPoint()
         */
        helios::engine::runtime::messaging::event::GameLoopEventBus passEventBus_{};


        /**
         * @brief Commits pass-level state based on the specified CommitPoint flags.
         *
         * @details Called after each pass that has a commit point configured. The
         * CommitPoint flags determine which synchronization actions are performed:
         *
         * - **PassEvents:** Swaps pass event bus buffers, making events pushed via
         *   `UpdateContext::pushPass()` readable in subsequent passes.
         * - **FlushCommands:** Executes pending commands from the CommandBuffer.
         * - **FlushManagers:** Processes manager requests (e.g., spawning from pools).
         *
         * The order is: PassEvents → FlushCommands → FlushManagers.
         * Commands must be flushed before managers to ensure spawn requests are
         * generated before being processed.
         *
         * @param commitPoint The flags specifying which actions to perform.
         * @param updateContext The current update context.
         *
         * @see CommitPoint
         * @see Pass::addCommitPoint()
         * @see UpdateContext::pushPass()
         * @see UpdateContext::readPass()
         */
        void passCommit(
            const CommitPoint commitPoint,
            helios::engine::runtime::world::UpdateContext& updateContext) {


            if ((commitPoint & CommitPoint::PassEvents) == CommitPoint::PassEvents)  {
                passEventBus_.swapBuffers();
            }

            // commands must be executed before Managers
            if ((commitPoint & CommitPoint::FlushCommands) == CommitPoint::FlushCommands) {
                commandBuffer_.flush(updateContext.gameWorld());
            }

            if ((commitPoint & CommitPoint::FlushManagers) == CommitPoint::FlushManagers) {
                updateContext.gameWorld().flushManagers(updateContext);
            }

        }

        /**
         * @brief Commits phase-level events and flushes commands and managers.
         *
         * Called after each phase completes. This method:
         * 1. Swaps phase event bus buffers, making events readable in the next phase.
         * 2. Clears pass event buffers for the new phase.
         * 3. Flushes the command buffer, executing deferred commands.
         * 4. Flushes managers, allowing to process any request generated by the commands.
         *
         * @param gameWorld Reference to the game world.
         * @param updateContext The current update context.
         *
         * @see UpdateContext::pushPhase()
         * @see UpdateContext::readPhase()
         */
        void phaseCommit(helios::engine::runtime::world::GameWorld& gameWorld, helios::engine::runtime::world::UpdateContext& updateContext) {
            phaseEventBus_.swapBuffers();
            passEventBus_.clearAll();

            // command buffer generates request for managers, so this comes first
            commandBuffer_.flush(updateContext.gameWorld());

            // managers process requests
            gameWorld.flushManagers(updateContext);
        }


    public:


        GameLoop() = default;

        /**
         * @brief Returns a reference to the specified phase.
         *
         * @param phaseType The type of phase to retrieve (Pre, Main, or Post).
         *
         * @return Reference to the requested Phase.
         */
        helios::engine::runtime::gameloop::Phase& phase(helios::engine::runtime::gameloop::PhaseType phaseType) {

            switch (phaseType) {
                case helios::engine::runtime::gameloop::PhaseType::Pre:
                    return prePhase_;
                    break;
                case helios::engine::runtime::gameloop::PhaseType::Main:
                    return mainPhase_;
                    break;
                case helios::engine::runtime::gameloop::PhaseType::Post:
                    return postPhase_;
                    break;
            }

            std::unreachable();

        }

        /**
         * @brief Returns a reference to the command buffer.
         *
         * @return Reference to the CommandBuffer owned by this GameLoop.
         */
        helios::engine::runtime::messaging::command::CommandBuffer& commandBuffer() {
            return commandBuffer_;
        }

        /**
         * @brief Initializes the GameLoop and all registered phases and passes.
         *
         * Must be called once before the first update(). Asserts if called multiple times.
         *
         * @param gameWorld Reference to the game world to initialize with.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) {

            assert(!initialized_ && "init() already called");

            for (auto phase : {helios::engine::runtime::gameloop::PhaseType::Pre,
                               helios::engine::runtime::gameloop::PhaseType::Main,
                               helios::engine::runtime::gameloop::PhaseType::Post}) {
                switch (phase) {
                    case helios::engine::runtime::gameloop::PhaseType::Pre:
                        prePhase_.init(gameWorld);
                        break;
                    case helios::engine::runtime::gameloop::PhaseType::Main:
                        mainPhase_.init(gameWorld);
                        break;
                    case helios::engine::runtime::gameloop::PhaseType::Post:
                        postPhase_.init(gameWorld);
                        break;
                }
            }

            initialized_ = true;
        }

        /**
         * @brief Executes one full frame update across all phases.
         *
         * Iterates through Pre, Main, and Post phases, updating all registered
         * systems and committing events and commands after each phase.
         *
         * @param gameWorld Reference to the game world.
         * @param deltaTime Time elapsed since the last frame in seconds.
         * @param inputSnapshot Snapshot of the current input state.
         */
        void update(helios::engine::runtime::world::GameWorld& gameWorld, float deltaTime, const helios::input::InputSnapshot& inputSnapshot) noexcept {

            assert(initialized_ && "GameLoop not initialized");

            auto updateContext = helios::engine::runtime::world::UpdateContext(
                  commandBuffer_,
                  gameWorld,
                  deltaTime,
                  phaseEventBus_,
                  passEventBus_,
                  inputSnapshot
              );


            // gameloop phases
            for (auto phase : {helios::engine::runtime::gameloop::PhaseType::Pre,
                               helios::engine::runtime::gameloop::PhaseType::Main,
                               helios::engine::runtime::gameloop::PhaseType::Post}) {

                switch (phase) {
                    case helios::engine::runtime::gameloop::PhaseType::Pre:
                        prePhase_.update(updateContext);
                        break;
                    case helios::engine::runtime::gameloop::PhaseType::Main:
                        mainPhase_.update(updateContext);
                        break;
                    case helios::engine::runtime::gameloop::PhaseType::Post:
                        postPhase_.update(updateContext);
                        break;
                }

                phaseCommit(gameWorld, updateContext);
            }
        }



    };

}

