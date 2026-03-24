/**
 * @file EngineCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with all engine command types.
 */
module;

#include <utility>
#include <type_traits>

export module helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.state.Bindings;

import helios.engine.state.commands;

import helios.engine.runtime.spawn.commands;

import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.modules.ui.widgets.commands;
import helios.engine.mechanics.timing.commands;
import helios.engine.mechanics.combat.commands;
import helios.engine.mechanics.damage.commands;
import helios.engine.mechanics.scoring.commands;
import helios.engine.modules.physics.motion.commands;
import helios.engine.mechanics.lifecycle.commands.WorldLifecycleCommand;

import helios.engine.common.tags.CommandBufferRole;

static_assert(requires {
  typename helios::engine::state::types::StateTransitionId<
    helios::engine::mechanics::match::types::MatchState
  >::Type;
}, "Bindings not visible in EngineCommandBuffer TU");

static_assert(requires {
  typename helios::engine::state::types::StateTransitionId<
    helios::engine::mechanics::gamestate::types::GameState
  >::Type;
}, "Bindings not visible in EngineCommandBuffer TU");

export namespace helios::engine::runtime::world {
    class UpdateContext;
    class GameWorld;
}

using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Concrete command buffer pre-configured with all engine command types.
     *
     * @details EngineCommandBuffer is a thin facade over a TypedCommandBuffer
     * instantiated with the full set of command types used by the engine.
     * It is registered as a resource in the GameWorld and wrapped by
     * a type-erased CommandBuffer via the Concept/Model pattern.
     * Systems access it via `UpdateContext::queueCommand<T>()`.
     *
     * ## Registered Command Types
     *
     * | Domain | Commands |
     * |--------|----------|
     * | Combat | `Aim2DCommand`, `ShootCommand`, `ApplyDamageCommand` |
     * | Motion | `Move2DCommand`, `SteeringCommand` |
     * | Scoring | `UpdateScoreCommand` |
     * | Spawn | `ScheduledSpawnPlanCommand`, `SpawnCommand`, `DespawnCommand` |
     * | State | `StateCommand<GameState>`, `DelayedStateCommand<GameState>`, `StateCommand<MatchState>`, `DelayedStateCommand<MatchState>` |
     * | UI | `UiActionCommand` |
     * | Timing | `TimerControlCommand` |
     * | Lifecycle | `WorldLifecycleCommand` |
     *
     * @see TypedCommandBuffer
     * @see CommandBuffer
     * @see UpdateContext
     */
    class EngineCommandBuffer {


        /**
         * @brief The underlying typed buffer with all engine command types.
         */
        using BufferImpl = helios::engine::runtime::messaging::command::TypedCommandBuffer<
            helios::engine::mechanics::combat::commands::Aim2DCommand,
            helios::engine::mechanics::combat::commands::ShootCommand,
            helios::engine::mechanics::damage::commands::ApplyDamageCommand,

            helios::engine::modules::physics::motion::commands::Move2DCommand,
            helios::engine::modules::physics::motion::commands::SteeringCommand,

            helios::engine::mechanics::scoring::commands::UpdateScoreCommand,

            helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand,
            helios::engine::runtime::spawn::commands::SpawnCommand,
            helios::engine::runtime::spawn::commands::DespawnCommand,
            helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>,
            helios::engine::state::commands::DelayedStateCommand<helios::engine::mechanics::gamestate::types::GameState>,
            helios::engine::state::commands::StateCommand<helios::engine::mechanics::match::types::MatchState>,
            helios::engine::state::commands::DelayedStateCommand<helios::engine::mechanics::match::types::MatchState>,
            helios::engine::modules::ui::widgets::commands::UiActionCommand,
            helios::engine::mechanics::timing::commands::TimerControlCommand,
            helios::engine::mechanics::lifecycle::commands::WorldLifecycleCommand
        >;

        /**
         * @brief The buffer implementation instance.
         */
        BufferImpl impl_;

    public:

        /**
         * @brief Tag identifying this type as a command buffer for ResourceRegistry.
         */
        using EngineRoleTag = helios::engine::common::tags::CommandBufferRole;

        /**
         * @brief Enqueues a command of the specified type.
         *
         * @tparam T The command type. Must be one of the registered command types.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the command constructor.
         */
        template<class T, class... Args>
        void add(Args&&... args) {
            impl_.template add<T>(std::forward<Args>(args)...);
        }


        /**
         * @brief Flushes all command queues via the underlying TypedCommandBuffer.
         *
         * @param gameWorld The game world where the commands are flushed.
         * @param updateContext The current frame's update context.
         */
        void flush(GameWorld& gameWorld, UpdateContext& updateContext) noexcept {
            impl_.flush(gameWorld, updateContext);
        }

        /**
         * @brief Discards all queued commands without executing them.
         */
        void clear() noexcept {
            impl_.clear();
        }
    };


}

