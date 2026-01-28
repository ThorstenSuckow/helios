/**
 * @file Phase.ixx
 * @brief Represents a phase within the game loop containing multiple passes.
 */
module;

#include <memory>
#include <vector>

export module helios.engine.runtime.gameloop.Phase;

import helios.engine.runtime.gameloop.PassCommitListener;
import helios.engine.runtime.gameloop.Pass;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.gameloop.CommitPoint;

export namespace helios::engine::runtime::gameloop {

    class GameLoop;


    /**
     * @brief Enumeration of game loop phase types.
     *
     * The game loop is divided into three sequential phases:
     * - **Pre:** Input processing and command generation.
     * - **Main:** Core gameplay simulation (physics, AI, game logic).
     * - **Post:** Synchronization, cleanup, and preparation for rendering.
     */
    enum class PhaseType {
        /**
         * @brief Pre-update phase for input and command processing.
         */
        Pre,

        /**
         * @brief Main update phase for core gameplay systems.
         */
        Main,

        /**
         * @brief Post-update phase for cleanup and scene synchronization.
         */
        Post
    };

    /**
     * @brief Represents a phase in the game loop containing multiple passes.
     *
     * A Phase groups related systems into passes that are executed sequentially.
     * Each pass can contain multiple systems and may have a commit point for
     * event synchronization.
     *
     * Commit points allow fine-grained control over when commands are flushed,
     * managers are processed, and pass-level events are synchronized. See
     * CommitPoint for available synchronization options.
     *
     * Phases are owned by the GameLoop and should not be created directly.
     *
     * @see Pass
     * @see CommitPoint
     * @see GameLoop
     */
    class Phase {

        friend class helios::engine::runtime::gameloop::GameLoop;

        /**
         * @brief Collection of listeners to be notified when a pass commits.
         *
         * @details Listeners are notified after each pass completes, receiving the
         * pass's configured CommitPoint flags. The GameLoop registers itself as a
         * listener to handle event buffer swapping, command flushing, and manager
         * processing based on the commit point configuration.
         *
         * @see PassCommitListener
         * @see notifyPassCommitListeners()
         */
        std::vector<PassCommitListener*> passCommitListeners_;



        /**
         * @brief Initializes all passes within this phase.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld){
            for (auto& pass : passEntries_) {
                // every pass contains systems that are updated here
                pass->init(gameWorld);
            }
        };

        /**
         * @brief Updates all passes within this phase.
         *
         * @details Iterates through all passes, updating their systems and then
         * invoking the commit action based on the pass's configured CommitPoint.
         * The commit point determines whether pass-level events are synchronized,
         * the command buffer is flushed, or managers are processed.
         *
         * @param updateContext The current update context.
         *
         * @see CommitPoint
         * @see Pass::addCommitPoint()
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext){

            for (auto& pass : passEntries_) {
                // every pass contains systems that are updated here
                pass->update(updateContext);
                notifyPassCommitListeners(pass->commitPoint(), updateContext);
            }
        };

        /**
         * @brief Notifies all registered listeners about a pass commit.
         *
         * @details Called after each pass completes its update cycle. Each registered
         * PassCommitListener receives the commit point flags, allowing it to perform
         * the appropriate synchronization actions (event swapping, command flushing,
         * manager processing).
         *
         * @param commitPoint The CommitPoint flags from the completed pass.
         * @param updateContext The current update context.
         *
         * @return Always returns true.
         *
         * @see PassCommitListener::onPassCommit()
         * @see addPassCommitListener()
         */
        bool notifyPassCommitListeners(CommitPoint commitPoint, helios::engine::runtime::world::UpdateContext& updateContext) {

            for (const auto & passCommitListener : passCommitListeners_) {
                passCommitListener->onPassCommit(commitPoint, updateContext);
            }
            return true;
        }

        /**
         * @brief Collection of passes belonging to this phase.
         */
        std::vector<std::unique_ptr<Pass>> passEntries_;

        /**
         * @brief Reference to the owning GameLoop.
         */
        helios::engine::runtime::gameloop::GameLoop& gameloop_;

    public:

        /**
         * @brief Constructs a Phase with a reference to its owning GameLoop.
         *
         * @param gameloop Reference to the parent GameLoop.
         */
        explicit Phase(helios::engine::runtime::gameloop::GameLoop& gameloop) : gameloop_(gameloop) {

        }


        /**
         * @brief Registers a listener to be notified when passes commit.
         *
         * @details The listener will receive notifications for all passes within this phase.
         * The GameLoop typically registers itself to handle event synchronization,
         * command buffer flushing, and manager processing based on commit point flags.
         *
         * Duplicate registrations are prevented; attempting to add the same listener
         * twice will return false and leave the listener list unchanged.
         *
         * @param passCommitListener Pointer to the listener to register. Must remain
         *        valid for the lifetime of this Phase or until removed.
         *
         * @return True if the listener was added, false if it was already registered.
         *
         * @see PassCommitListener
         * @see notifyPassCommitListeners()
         */
        bool addPassCommitListener(PassCommitListener* passCommitListener) {

            for (int i = 0; i < passCommitListeners_.size(); i++) {
                if (passCommitListeners_[i] == passCommitListener) {
                    return false;
                }
            }

            passCommitListeners_.emplace_back(passCommitListener);

            return true;
        }

        /**
         * @brief Creates and adds a new pass to this phase.
         *
         * @return Reference to the newly created Pass for method chaining.
         */
        Pass& addPass() {

            auto entry = std::make_unique<Pass>(*this);
            auto* raw = entry.get();
            passEntries_.emplace_back(std::move(entry));

            return *raw;
        }

        /**
         * @brief Returns a reference to the owning GameLoop.
         *
         * @return Reference to the parent GameLoop.
         */
        [[nodiscard]] helios::engine::runtime::gameloop::GameLoop& gameLoop()  {
            return gameloop_;
        }


    };

}