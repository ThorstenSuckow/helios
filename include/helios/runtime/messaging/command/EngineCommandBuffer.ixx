/**
 * @file EngineCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with all engine command types.
 */
module;

export module helios.runtime.messaging.command.EngineCommandBuffer;


import helios.runtime.spawn.commands;

import helios.platform.environment.types;
import helios.platform.environment.commands;
import helios.platform.window.commands;


import helios.platform.lifecycle.commands;


import helios.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.modules.ui.widgets.commands;
import helios.engine.mechanics.timing.commands;
import helios.engine.mechanics.combat.commands;
import helios.engine.mechanics.damage.commands;
import helios.engine.mechanics.scoring.commands;
import helios.engine.modules.physics.motion.commands;
import helios.engine.mechanics.lifecycle.commands.WorldLifecycleCommand;

import helios.rendering.shader.commands;
import helios.platform.window.types.WindowHandle;
import helios.runtime.world.types;
import helios.engine.common.tags.CommandBufferRole;

using namespace helios::platform::environment::types;
using namespace helios::platform::window::types;
using namespace helios::runtime::world;
using namespace helios::runtime::world::types;
export namespace helios::runtime::messaging::command {

    using EngineCommandBuffer = helios::runtime::messaging::command::TypedCommandBuffer<

            helios::engine::modules::physics::motion::commands::Move2DCommand<GameObjectHandle>,
            helios::engine::modules::physics::motion::commands::SteeringCommand<GameObjectHandle>,
            helios::engine::modules::ui::widgets::commands::UiActionCommand<GameObjectHandle>,

            helios::engine::mechanics::scoring::commands::UpdateScoreCommand,
            helios::engine::mechanics::timing::commands::TimerControlCommand,
            helios::engine::mechanics::lifecycle::commands::WorldLifecycleCommand,
            helios::engine::mechanics::combat::commands::Aim2DCommand<GameObjectHandle>,
            helios::engine::mechanics::combat::commands::ShootCommand<GameObjectHandle>,
            helios::engine::mechanics::damage::commands::ApplyDamageCommand<GameObjectHandle>,

            helios::runtime::spawn::commands::ScheduledSpawnPlanCommand<GameObjectHandle>,
            helios::runtime::spawn::commands::SpawnCommand<GameObjectHandle>,
            helios::runtime::spawn::commands::DespawnCommand<GameObjectHandle>,


            // window
            helios::platform::window::commands::WindowCreateCommand<WindowHandle>,
            helios::platform::window::commands::WindowResizeCommand<WindowHandle>,
            helios::platform::window::commands::SwapBuffersCommand<WindowHandle>,
            helios::platform::window::commands::WindowCloseCommand<WindowHandle>,

            // runtime platform
            helios::platform::lifecycle::commands::PlatformInitCommand,
            helios::platform::environment::commands::PollEventsCommand,
            helios::platform::lifecycle::commands::ShutdownCommand
        >;

}

