/**
 * @file Phase.ixx
 * @brief Represents a phase within the game loop containing multiple passes.
 */
module;

#include <memory>
#include <vector>

export module helios.engine.runtime.gameloop.Phase;

import helios.engine.runtime.gameloop.Pass;
import helios.engine.ecs.UpdateContext;
import helios.engine.ecs.GameWorld;

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
     * Phases are owned by the GameLoop and should not be created directly.
     *
     * @see Pass
     * @see GameLoop
     */
    class Phase {

        friend class helios::engine::runtime::gameloop::GameLoop;

        /**
         * @brief Initializes all passes within this phase.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(helios::engine::ecs::GameWorld& gameWorld){
            for (auto& pass : passEntries_) {
                // every pass contains systems that are updated here
                pass->init(gameWorld);
            }
        };

        /**
         * @brief Updates all passes within this phase.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::ecs::UpdateContext& updateContext){
            for (auto& pass : passEntries_) {
                // every pass contains systems that are updated here
                pass->update(updateContext);

                // once all systems were updated, we commit
                pass->commit();
            }
        };

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