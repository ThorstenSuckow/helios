/**
 * @file CommandBuffer.ixx
 * @brief Type-erased command buffer wrapper using the Concept/Model pattern.
 */
module;

#include <cassert>
#include <memory>

export module helios.runtime.messaging.command.CommandBuffer;


import helios.runtime.world.GameWorldFwd;
import helios.runtime.world.UpdateContextFwd;

import helios.engine.common.concepts.IsCommandBufferLike;

using namespace helios::engine::common::concepts;

using namespace helios::runtime::world;
export namespace helios::runtime::messaging::command {

    /**
     * @brief Type-erased wrapper for command buffers using the Concept/Model pattern.
     *
     * @details CommandBuffer erases the concrete buffer type at construction time,
     * allowing heterogeneous storage and polymorphic dispatch without requiring
     * concrete buffers to inherit from a common base class.
     *
     * ## Type Erasure
     *
     * ```
     * CommandBuffer (value type, owns erased Concept via unique_ptr)
     *   └── Concept              (internal virtual interface)
     *         └── Model<T>       (typed wrapper, owns T by value)
     *               └── T        (plain class, provides flush() / clear())
     * ```
     *
     * The wrapped type must satisfy `IsCommandBufferLike<T>`, which requires
     * `flush(GameWorld&, UpdateContext&) noexcept` and `clear() noexcept`.
     *
     * CommandBuffer is move-only (non-copyable).
     *
     * @see IsCommandBufferLike
     * @see TypedCommandBuffer
     * @see EngineCommandBuffer
     * @see ResourceRegistry
     */
    class CommandBuffer {

    private:

        /**
         * @brief Internal virtual interface for type-erased dispatch.
         */
        class Concept {
        public:
            virtual ~Concept() = default;
            virtual void flush(GameWorld& gameWorld, UpdateContext& updateContext) noexcept = 0;
            virtual void clear() noexcept = 0;

            [[nodiscard]] virtual void* underlying() noexcept = 0;
            [[nodiscard]] virtual const void* underlying() const noexcept = 0;
        };

        /**
         * @brief Typed model that adapts a concrete buffer to the Concept interface.
         *
         * @tparam T The concrete command buffer type.
         */
        template<typename T>
        class Model final : public Concept {

            /**
             * @brief The owned command buffer instance.
             */
            T cmdBuffer_;

            public:

            explicit Model(T cmdBuffer) :  cmdBuffer_(std::move(cmdBuffer)) {}

            void flush(GameWorld& gameWorld, UpdateContext& updateContext) noexcept override {
                cmdBuffer_.flush(gameWorld, updateContext);
            }

            void clear() noexcept override {
                cmdBuffer_.clear();
            }

            void* underlying() noexcept override {
                return &cmdBuffer_;
            }

            const void* underlying() const noexcept override {
                return &cmdBuffer_;
            }
        };

        /**
         * @brief Owning pointer to the type-erased command buffer.
         */
        std::unique_ptr<Concept> pimpl_;

    public:

        CommandBuffer() = default;

        /**
         * @brief Constructs a CommandBuffer wrapping the given concrete buffer.
         *
         * @details Ownership of the buffer is transferred into a heap-allocated
         * Model<T>. The concrete type is erased after construction.
         *
         * @tparam T The concrete buffer type. Must satisfy IsCommandBufferLike.
         *
         * @param cmdBuffer The buffer instance to wrap. Moved into the wrapper.
         */
        template<typename T>
        requires IsCommandBufferLike<T>
        explicit CommandBuffer(T cmdBuffer) : pimpl_(std::make_unique<Model<T>>(std::move(cmdBuffer))) {}

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;

        CommandBuffer& operator=(CommandBuffer&&) = default;
        CommandBuffer(CommandBuffer&&) noexcept = default;

        /**
         * @brief Executes all queued commands and clears the buffer.
         *
         * @details Delegates to the wrapped buffer's `flush()`. Commands are
         * routed to their registered handler if available, otherwise executed
         * directly.
         *
         * @param gameWorld The game world providing the CommandHandlerRegistry.
         * @param updateContext The current frame's update context.
         *
         * @pre The CommandBuffer must be initialized (not default-constructed).
         */
        void flush(GameWorld& gameWorld, UpdateContext& updateContext) noexcept {
            assert(pimpl_ && "CommandBuffer not initialized");
            pimpl_->flush(gameWorld, updateContext);
        }

        /**
         * @brief Discards all queued commands without executing them.
         *
         * @pre The CommandBuffer must be initialized.
         */
        void clear() noexcept {
            assert(pimpl_ && "CommandBuffer not initialized");
            pimpl_->clear();
        }

        /**
         * @brief Returns a type-erased pointer to the owned buffer instance.
         *
         * @return Non-null void pointer to the underlying concrete buffer.
         *
         * @pre The CommandBuffer must be initialized.
         */
        [[nodiscard]] void* underlying() noexcept {
            assert(pimpl_ && "CommandBuffer not initialized");
            return pimpl_->underlying();
        }

        /**
         * @copydoc underlying()
         */
        [[nodiscard]] const void* underlying() const noexcept {
            assert(pimpl_ && "CommandBuffer not initialized");
            return pimpl_->underlying();
        }

    };


}

