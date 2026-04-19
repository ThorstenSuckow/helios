/**
 * @file NullCommandBuffer.ixx
 * @brief No-op command buffer implementation used as a default command-buffer type.
 */
module;

export module helios.runtime.messaging.command.NullCommandBuffer;

import helios.runtime.messaging.command.tags.CommandBufferRole;
import helios.runtime.world.GameWorldFwd;
import helios.runtime.world.UpdateContextFwd;

using namespace helios::runtime::tags;
using namespace helios::runtime::world;
export namespace helios::runtime::messaging::command {

    /**
     * @brief Command buffer implementation that intentionally performs no work.
     *
     * Use this type as a default template argument when command emission is optional.
     */
    class NullCommandBuffer {

    public:

        using EngineRoleTag = helios::runtime::tags::CommandBufferRole;

        template<class T, class... Args>
        void add(Args&&... args) {/*intentionally noop*/}

        void flush(GameWorld& gameWorld, UpdateContext& updateContext) noexcept {/*intentionally noop*/}

        void clear() noexcept {/*intentionally noop*/}
    };


}
