/**
 * @file TypedPass.ixx
 * @brief State-filtered pass implementation for game loop phases.
 */
module;

#include <type_traits>
#include <utility>

export module helios.engine.runtime.gameloop.TypedPass;

import helios.engine.runtime.gameloop.CommitPoint;

import helios.engine.runtime.gameloop.Pass;

import helios.engine.ecs.System;
import helios.engine.runtime.world.SystemRegistry;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.Session;

import helios.engine.mechanics.gamestate.types;

export namespace helios::engine::modules {
    class GameWorld;
}

export namespace helios::engine::runtime::gameloop {

    class Phase;

    /**
     * @brief State-filtered pass that only executes in specific states.
     *
     * @details TypedPass extends the base Pass class with state-based filtering.
     * The pass only executes when the current state (queried from Session)
     * matches the configured state mask using bitwise AND.
     *
     * ## State Filtering
     *
     * The state mask is a bitfield where each bit represents a state value.
     * A pass runs if any bit in the mask matches the current state:
     *
     * ```cpp
     * // Run only in Running state
     * phase.addPass<GameState>(GameState::Running)
     *     .addSystem<MovementSystem>();
     *
     * // Run in multiple states (bitwise OR)
     * phase.addPass<GameState>(GameState::Running | GameState::Paused)
     *     .addSystem<InputSystem>();
     * ```
     *
     * @tparam StateType The state enum type (e.g., GameState, MatchState).
     *
     * @see Pass
     * @see Phase::addPass()
     * @see Session::state()
     */
    template<typename StateType>
    class TypedPass : public Pass {

        friend class helios::engine::runtime::gameloop::Phase;

        /**
         * @brief Reference to the owning Phase.
         */
        Phase& owner_;

        /**
         * @brief Bitmask of states in which this pass should execute.
         */
        StateType mask_;

        /**
         * @brief Updates all systems registered in this pass.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) override {
            for (auto& sys : systemRegistry_.systems()) {
                sys->update(updateContext);
            }
        }


        /**
         * @brief Initializes all systems registered in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {
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
        explicit TypedPass(Phase& owner, const StateType mask) : owner_(owner), mask_(mask) {}


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
        Phase& addCommitPoint(const CommitPoint commitPoint = CommitPoint::PassEvents) override {
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
        [[nodiscard]] CommitPoint commitPoint() const noexcept override {
            return commitPoint_;
        }

        /**
         * @brief Checks if this pass should execute based on current state.
         *
         * @details Queries the current state from the Session and compares it
         * against the configured mask using bitwise AND. The pass runs if
         * any bit in the mask matches the current state.
         *
         * @param updateContext The current update context.
         *
         * @return True if the pass should execute.
         */
        [[nodiscard]] bool shouldRun(helios::engine::runtime::world::UpdateContext& updateContext) const noexcept override {
            auto state = updateContext.session().state<StateType>();
            bool runs = hasFlag(mask_, state);
            if (!runs) {
                return false;
            }
            return runs;
        }

        /**
         * @brief Checks if a value has any bit set in the mask.
         *
         * @param mask The bitmask to check against.
         * @param value The value to test.
         *
         * @return True if any bit in mask matches value.
         */
        bool hasFlag(StateType mask, StateType value) const noexcept {
            using U = std::underlying_type_t<StateType>;
            return (static_cast<U>(mask) & static_cast<U>(value)) != 0;
        }


    };

}