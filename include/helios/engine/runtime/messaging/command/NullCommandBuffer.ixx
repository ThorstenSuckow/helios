/**
 * @file NullCommandBuffer.ixx
 * @brief No-op command buffer implementation used as a default command-buffer type.
 */
module;

export module helios.engine.runtime.messaging.command.NullCommandBuffer;

import helios.engine.common.tags;
import helios.engine.runtime.world.GameWorldFwd;
import helios.engine.runtime.world.UpdateContextFwd;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Command buffer implementation that intentionally performs no work.
     *
     * Use this type as a default template argument when command emission is optional.
     */
    class NullCommandBuffer {

    public:

        using EngineRoleTag = helios::engine::common::tags::CommandBufferRole;

        template<class T, class... Args>
        void add(Args&&... args) {/*intentionally noop*/}

        void flush(GameWorld& gameWorld, UpdateContext& updateContext) noexcept {/*intentionally noop*/}

        void clear() noexcept {/*intentionally noop*/}
    };


}
