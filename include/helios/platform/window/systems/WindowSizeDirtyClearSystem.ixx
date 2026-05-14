module;

#include <cassert>

export module helios.platform.window.systems.WindowSizeDirtyClearSystem;

import helios.runtime.world.tags.SystemRole;
import helios.runtime.world.UpdateContext;

import helios.platform.window.components;

import helios.ecs.components.Active;
import helios.platform.window.concepts.IsWindowHandle;

import helios.spatial.components.Size2DComponent;

using namespace helios::spatial::components;
using namespace helios::runtime::world::tags;
using namespace helios::platform::window::concepts;
using namespace helios::runtime::world;
using namespace helios::platform::window::components;
using namespace helios::ecs::components;
export namespace helios::platform::window::systems {

    template<typename TMemberHandle>
    requires IsWindowHandle<TMemberHandle>
    class WindowSizeDirtyClearSystem {

    public:

        using EngineRoleTag = SystemRole;

        void update(UpdateContext& updateContext) noexcept {
            for (auto [entity, wc, wsc, active] : updateContext.view<
                TMemberHandle,
                WindowComponent<TMemberHandle>,
                Size2DComponent<TMemberHandle>,
                Active<TMemberHandle>
            >().whereEnabled()) {
                wsc->clearDirty();
            }
        };

    };

}