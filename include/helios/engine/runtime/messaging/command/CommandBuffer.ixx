/**
 * @file CommandBuffer.ixx
 * @brief Abstract base class for command buffers.
 */
module;

#include <utility>
#include <type_traits>

export module helios.engine.runtime.messaging.command.CommandBuffer;

export namespace helios::engine::runtime::world {
    class UpdateContext;
    class GameWorld;
}

using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Abstract base for command buffers that collect and flush commands.
     *
     * @details CommandBuffer defines the interface for deferred command execution.
     * Commands are accumulated during the frame and flushed at defined commit
     * points in the game loop. Concrete implementations (e.g., TypedCommandBuffer)
     * provide type-safe storage and routing.
     *
     * @see TypedCommandBuffer
     * @see EngineCommandBuffer
     * @see Manager
     */
    class CommandBuffer {


    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~CommandBuffer() = default;

        /**
         * @brief Executes all queued commands and clears the buffer.
         *
         * @details Commands are routed to their registered handler
         * if available, otherwise executed directly via their `execute()` method.
         *
         * @param gameWorld The game world where the commands are flushed.
         * @param updateContext The current frame's update context.
         */
        virtual void flush(GameWorld& gameWorld, UpdateContext& updateContext) noexcept = 0;

        /**
         * @brief Discards all queued commands without executing them.
         */
        virtual void clear() noexcept = 0;
    };




}
