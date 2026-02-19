/**
 * @file Pass.ixx
 * @brief Abstract base class for game loop passes.
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
     * @brief Abstract base class for game loop passes.
     *
     * @details A Pass represents a logical grouping of systems executed
     * sequentially within a Phase. Concrete implementations (TypedPass)
     * add state-based filtering via shouldRun().
     *
     * ## Key Features
     *
     * - **System Registration:** Systems are added via addSystem<T>()
     * - **Commit Points:** Control when events/commands are synchronized
     * - **State Filtering:** Passes can be skipped based on game state
     *
     * @see TypedPass
     * @see Phase
     * @see System
     */
    class Pass {

    protected:
        /**
         * @brief Registry holding all systems for this pass.
         */
        helios::engine::runtime::world::SystemRegistry systemRegistry_{};

    public:
        virtual ~Pass() = default;

        /**
         * @brief Updates all systems in this pass.
         *
         * @param updateContext The current update context.
         */
        virtual void update(helios::engine::runtime::world::UpdateContext& updateContext) = 0;

        /**
         * @brief Initializes all systems in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        virtual void init(helios::engine::runtime::world::GameWorld& gameWorld) = 0;

        /**
         * @brief Adds a commit point and returns the owning Phase.
         *
         * @param commitPoint The synchronization flags.
         *
         * @return Reference to the owning Phase.
         */
        virtual Phase& addCommitPoint(const CommitPoint commitPoint = CommitPoint::PassEvents) = 0;

        /**
         * @brief Returns the configured commit point.
         *
         * @return The commit point flags for this pass.
         */
        virtual CommitPoint commitPoint() const noexcept = 0;

        /**
         * @brief Determines if this pass should execute.
         *
         * @param updateContext The current update context.
         *
         * @return True if the pass should run.
         */
        virtual bool shouldRun(helios::engine::runtime::world::UpdateContext& updateContext) const noexcept = 0;

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
    };

}