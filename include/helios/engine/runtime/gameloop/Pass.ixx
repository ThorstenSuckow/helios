/**
 * @file Pass.ixx
 * @brief Represents a single pass within a phase, containing registered systems.
 */
module;

#include <type_traits>
#include <utility>

export module helios.engine.runtime.gameloop.Pass;

import helios.engine.ecs.System;
import helios.engine.ecs.SystemRegistry;

import helios.engine.ecs.UpdateContext;


export namespace helios::engine::game {
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
        helios::engine::ecs::SystemRegistry systemRegistry_{};

        /**
         * @brief Reference to the owning Phase.
         */
        Phase& owner_;

        /**
         * @brief Updates all systems registered in this pass.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::ecs::UpdateContext& updateContext) ;

        /**
         * @brief Commits pass-level events if a commit point is configured.
         */
        void commit();

        /**
         * @brief Initializes all systems registered in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(helios::engine::ecs::GameWorld& gameWorld);


        /**
         * @brief Flag indicating whether this pass has a commit point.
         */
        bool hasCommitPoint_ = false;

        public:

        /**
         * @brief Constructs a Pass with a reference to its owning Phase.
         *
         * @param owner Reference to the parent Phase.
         */
        explicit Pass(
            Phase& owner
        ) : owner_(owner) {}

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
        Pass& add(Args&&... args) {
            systemRegistry_.add<T>(std::forward<Args>(args)...);

            return *this;
        }

        /**
         * @brief Marks this pass with a commit point and returns the owning Phase.
         *
         * When a commit point is set, pass-level events are synchronized after
         * this pass completes, making them available to subsequent passes.
         *
         * Events pushed via `UpdateContext::pushPass()` become readable in the next
         * pass through `UpdateContext::readPass()`. The event buses are configured
         * in GameLoop and passed to systems via UpdateContext.
         *
         * @return Reference to the owning Phase for continued configuration.
         *
         * @see UpdateContext::pushPass()
         * @see UpdateContext::readPass()
         * @see GameLoop
         */
        Phase& addCommitPoint() {
            hasCommitPoint_ = true;
            return owner_;
        }



    };

}