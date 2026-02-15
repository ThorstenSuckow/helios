/**
 * @file Pass.ixx
 * @brief Represents a single pass within a phase, containing registered systems.
 */
module;

#include <type_traits>
#include <utility>

export module helios.engine.runtime.gameloop.Pass;

import helios.engine.runtime.gameloop.CommitPoint;

import helios.engine.ecs.System;
import helios.engine.runtime.world.SystemRegistry;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.gamestate.types;

export namespace helios::engine::modules {
    class GameWorld;
}

export namespace helios::engine::runtime::gameloop {

    class Phase;

    /**
     * @brief Represents a single pass within a game loop phase.
     *
     * A Pass contains a registry of systems that are executed sequentially during
     * the game loop update. Passes can be configured with a commit point to
     * synchronize events between passes.
     *
     * Systems are added to a pass using the fluent `add<T>()` method, and
     * a commit point can be added using `addCommitPoint()`.
     *
     * Example usage:
     * ```cpp
     * gameLoop.phase(PhaseType::Main)
     *     .addPass()
     *         .add<MovementSystem>()
     *         .add<PhysicsSystem>()
     *         .addCommitPoint()
     *     .addPass()
     *         .add<CollisionSystem>();
     * ```
     *
     * @see Phase
     * @see System
     */
    class Pass {

        friend class helios::engine::runtime::gameloop::Phase;

        /**
         * @brief Registry holding all systems for this pass.
         */
        helios::engine::runtime::world::SystemRegistry systemRegistry_{};

        /**
         * @brief Reference to the owning Phase.
         */
        Phase& owner_;

        /**
         * @brief The game state(s) in which this pass should execute.
         */
        helios::engine::mechanics::gamestate::types::GameState gameState_;

        /**
         * @brief Updates all systems registered in this pass.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) {
            for (auto& sys : systemRegistry_.systems()) {
                sys->update(updateContext);
            }
        }


        /**
         * @brief Initializes all systems registered in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) {
            for (auto& sys : systemRegistry_.systems()) {
                sys->init(gameWorld);
            }
        }


        /**
         * @brief The CommitPoint configured for this Pass.
         */
        CommitPoint commitPoint_ = CommitPoint::None;



        public:

        /**
         * @brief Constructs a Pass with a reference to its owning Phase.
         *
         * @param owner Reference to the parent Phase.
         */
        explicit Pass(
            Phase& owner,
            const helios::engine::mechanics::gamestate::types::GameState gameState
        ) : owner_(owner), gameState_(gameState) {}

        /**
         * @brief Adds a system of type T to this pass.
         *
         * @tparam T The system type to add. Must derive from System.
         * @tparam Args Constructor argument types for the system.
         *
         * @param args Arguments forwarded to the system constructor.
         *
         * @return Reference to this Pass for method chaining.
         */
        template<typename T, typename... Args>
        requires std::is_base_of_v<helios::engine::ecs::System, T>
        Pass& addSystem(Args&&... args) {
            systemRegistry_.add<T>(std::forward<Args>(args)...);

            return *this;
        }

        /**
         * @brief Marks this pass with a commit point and returns the owning Phase.
         *
         * @details When a commit point is set, the specified synchronization actions are
         * performed after this pass completes. The default is `CommitPoint::PassEvents`
         * which only synchronizes pass-level events.
         *
         * Available CommitPoint flags:
         * - `PassEvents` — Events pushed via `UpdateContext::pushPass()` become readable.
         * - `FlushCommands` — Pending commands from the CommandBuffer are executed.
         * - `FlushManagers` — Managers process their queued requests.
         * - `Structural` — Combines all three flags.
         *
         * Flags can be combined using bitwise OR:
         * ```cpp
         * pass.addCommitPoint(CommitPoint::PassEvents | CommitPoint::FlushCommands);
         * ```
         *
         * @param commitPoint The flags specifying which actions to perform (default: PassEvents).
         *
         * @return Reference to the owning Phase for continued configuration.
         *
         * @see CommitPoint
         * @see UpdateContext::pushPass()
         * @see UpdateContext::readPass()
         * @see GameLoop::passCommit()
         */
        Phase& addCommitPoint(const CommitPoint commitPoint = CommitPoint::PassEvents) {
            commitPoint_ = commitPoint;
            return owner_;
        }


        /**
         * @brief Returns the configured commit point for this pass.
         *
         * @details The commit point determines what synchronization actions are performed
         * after this pass completes. If no commit point was added, returns CommitPoint::None.
         *
         * @return The commit point flags for this pass.
         *
         * @see CommitPoint
         * @see addCommitPoint()
         */
        [[nodiscard]] CommitPoint commitPoint() const noexcept {
            return commitPoint_;
        }

        /**
         * @brief Returns the game state(s) in which this pass executes.
         *
         * @details Passes can be configured to run only during specific game states
         * (e.g., Playing, Paused). The pass will be skipped if the current game state
         * does not match the configured state mask.
         *
         * @return The game state bitmask for this pass.
         *
         * @see GameState
         */
        [[nodiscard]] helios::engine::mechanics::gamestate::types::GameState runsIn() {
            return gameState_;
        }


    };

}