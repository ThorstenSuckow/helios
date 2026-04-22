/**
 * @file Pass.ixx
 * @brief Abstract base class for game loop passes.
 */
module;

#include <type_traits>
#include <utility>
#include <memory>
#include <cassert>

export module helios.runtime.gameloop:Pass;

import :CommitPoint;

import helios.runtime.world.GameWorld;
import helios.runtime.world.SystemRegistry;
import helios.runtime.world.System;
import helios.runtime.world.concepts;


import helios.runtime.world.UpdateContext;

import helios.gameplay.gamestate.types;

using namespace helios::runtime::world;
export namespace helios::runtime::gameloop {

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
        helios::runtime::world::SystemRegistry systemRegistry_{};

        /**
         * @brief Reference to the owning GameWorld.
         *
         * @details Used to resolve optional command buffer dependencies for
         * systems declaring `CommandBuffer_type`.
         */
        GameWorld& gameWorld_;


    public:
        virtual ~Pass() = default;

        /**
         * @brief Constructs a pass bound to a specific GameWorld.
         *
         * @param gameWorld GameWorld used for system initialization and
         *        command buffer lookup.
         */
        explicit Pass(GameWorld& gameWorld) : gameWorld_(gameWorld) {};

        /**
         * @brief Updates all systems in this pass.
         *
         * @param updateContext The current update context.
         */
        virtual void update(helios::runtime::world::UpdateContext& updateContext) = 0;

        /**
         * @brief Initializes all systems in this pass.
         *
         * @param gameWorld Reference to the game world.
         */
        virtual void init(helios::runtime::world::GameWorld& gameWorld) = 0;

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
        virtual bool shouldRun(helios::runtime::world::UpdateContext& updateContext) const noexcept = 0;

        /**
         * @brief Adds a system of type T to this pass.
         *
         * @tparam T The system type to add.
         * @tparam Args Constructor argument types for the system.
         *
         * @param args Arguments forwarded to the system constructor.
         *
         * @details If `T` defines `CommandBuffer_type`, the buffer is resolved
         * from the bound `GameWorld` and injected into the wrapped
         * `helios::runtime::world::System`.
         *
         * @return Reference to this Pass for method chaining.
         */
        template<typename T, typename... Args>
        requires helios::runtime::world::concepts::IsSystemLike<T>
        Pass& addSystem(Args&&... args) {

            T concreteSystem(std::forward<Args>(args)...);

            void* bufferPtr = nullptr;
            if constexpr (requires { typename T::CommandBuffer_type; }) {
                using TCommandBuffer = typename T::CommandBuffer_type;
                bufferPtr = gameWorld_.tryCommandBuffer<TCommandBuffer>();
                assert(bufferPtr && "Command buffer not found for system's CommandBuffer_type");
            }

            systemRegistry_.template add<T>(
                System(std::move(concreteSystem), bufferPtr)
            );

            return *this;
        }
    };

}