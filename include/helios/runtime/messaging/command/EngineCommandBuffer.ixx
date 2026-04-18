/**
 * @file EngineCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with all engine command types.
 */
module;

export module helios.runtime.messaging.command.EngineCommandBuffer;


import helios.gameplay.spawn.commands;

import helios.platform.environment.types;
import helios.platform.environment.commands;
import helios.platform.window.commands;


import helios.platform.lifecycle.commands;


import helios.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.modules.ui.widgets.commands;
import helios.gameplay.timing.commands;
import helios.gameplay.combat.commands;
import helios.gameplay.damage.commands;
import helios.gameplay.scoring.commands;
import helios.engine.modules.physics.motion.commands;
import helios.gameplay.lifecycle.commands.WorldLifecycleCommand;

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

            helios::gameplay::scoring::commands::UpdateScoreCommand,
            helios::gameplay::timing::commands::TimerControlCommand,
            helios::gameplay::lifecycle::commands::WorldLifecycleCommand,
            helios::gameplay::combat::commands::Aim2DCommand<GameObjectHandle>,
            helios::gameplay::combat::commands::ShootCommand<GameObjectHandle>,
            helios::gameplay::damage::commands::ApplyDamageCommand<GameObjectHandle>,

            helios::gameplay::spawn::commands::ScheduledSpawnPlanCommand<GameObjectHandle>,
            helios::gameplay::spawn::commands::SpawnCommand<GameObjectHandle>,
            helios::gameplay::spawn::commands::DespawnCommand<GameObjectHandle>,


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

