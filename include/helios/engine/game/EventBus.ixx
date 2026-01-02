/**
 * @file EventBus.ixx
 * @brief Double-buffered event system for decoupled inter-system communication.
 */
module;

#include <memory>
#include <span>
#include <vector>

export module helios.engine.game.EventBus;

import helios.core.TypeIndexer;

namespace {
    /**
     * @class EventBufferBase
     * @brief Abstract base class for type-erased event buffers.
     *
     * @details Provides a common interface for buffer operations (swap, clear)
     * without exposing the concrete event type. Used internally by EventBus
     * to manage heterogeneous event buffers in a single container.
     */
    class EventBufferBase {

    public:
        EventBufferBase() = default;

        virtual ~EventBufferBase() = default;

        /**
         * @brief Swaps the read and write buffers.
         *
         * @details Clears the current read buffer, then exchanges it with the write buffer.
         * After this call, previously written events become available for reading.
         */
        virtual void swap() = 0;

        /**
         * @brief Clears all events from the read buffer.
         */
        virtual void clearReadBuffer() = 0;

        /**
         * @brief Clears all events from the write buffer.
         */
        virtual void clearWriteBuffer() = 0;

    };
}

export namespace helios::engine::game {


    /**
     * @class EventBuffer
     * @brief Type-safe double-buffered container for events of type T.
     *
     * @details Implements a producer-consumer pattern where events are written
     * to one buffer while consumers read from another. This decouples event
     * production from consumption and allows lock-free operation in single-threaded
     * game loops.
     *
     * @tparam T The event type stored in this buffer. Must be movable.
     */
    template<typename T>
    class EventBuffer : public EventBufferBase {

        /**
         * @brief Buffer containing events available for reading.
         */
        std::vector<T> readBuffer_;

        /**
         * @brief Buffer where new events are pushed.
         */
        std::vector<T> writeBuffer_;


    public:

        /**
         * @brief Pre-allocates memory for both buffers.
         *
         * @param size The number of events to reserve capacity for.
         */
        void reserve(size_t size) {
            readBuffer_.reserve(size);
            writeBuffer_.reserve(size);
        }

        /**
         * @brief Constructs and pushes an event to the write buffer.
         *
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to T's constructor.
         */
        template<typename... Args>
        void push(Args&&... args) {
            writeBuffer_.emplace_back(std::forward<Args>(args)...);
        }

        /**
         * @brief Returns a read-only view of events in the read buffer.
         *
         * @return A span over all events written in the previous frame.
         */
        std::span<const T> read() const noexcept {
            return readBuffer_;
        }

        /**
         * @brief Swaps read with write buffer.
         *
         * @details Swaps the read- with the write-buffer. The write buffer is empty after
         * this operation, while the read buffer contains the contents of the write buffer.
         */
        void swap() override {
            clearReadBuffer();
            readBuffer_.swap(writeBuffer_);
        }

        /**
         * @brief Clears the read buffer.
         */
        void clearReadBuffer() override {
            readBuffer_.clear();
        }

        /**
         * @brief Clears the write buffer.
         */
        void clearWriteBuffer() override {
            writeBuffer_.clear();
        }
    };

    /**
     * @class EventBus
     * @brief Central hub for publishing and subscribing to typed events.
     *
     * @details The EventBus provides a double-buffered, type-indexed event system.
     * Systems push events during their update phase, then at frame boundaries
     * `swapBuffers()` is called to make those events available for reading.
     *
     * Each event type T gets its own dedicated buffer, indexed via TypeIndexer
     * for O(1) access.
     *
     * Example usage:
     * ```cpp
     * // Define an event
     * struct CollisionEvent {
     *     EntityId a;
     *     EntityId b;
     *     vec3f contact;
     * };
     *
     * // In collision system
     * updateContext.pushEvent<CollisionEvent>(entityA, entityB, contactPoint);
     *
     * // At frame boundary
     * eventBus.swapBuffers();
     *
     * // In damage system (next phase or frame)
     * for (const auto& evt : eventBus.read<CollisionEvent>()) {
     *     applyDamage(evt.a, evt.b);
     * }
     * ```
     */
    class EventBus {

        /**
         * @brief Tag type for TypeIndexer to create an event-specific index space.
         */
        struct EventGroup{};

        /**
         * @brief Type alias for the event type indexer.
         */
        using TypeIndexGenerator = helios::core::TypeIndexer<EventGroup>;

        /**
         * @brief Storage for type-erased event buffers, indexed by type.
         */
        std::vector<std::unique_ptr<EventBufferBase>> buffers_;

    public:

        ~EventBus() = default;
        EventBus() = default;

        /**
         * @brief Pushes an event of type T to the write buffer.
         *
         * @tparam T The event type.
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to T's constructor.
         */
        template<typename T, typename... Args>
        void push(Args&&... args) {
            getOrCreateBuffer<T>().push(std::forward<Args>(args)...);
        }

        /**
         * @brief Returns a read-only view of all events of type T.
         *
         * @details Returns events that were written before the last `swapBuffers()` call.
         * If no events of type T exist, returns an empty span.
         *
         * @tparam T The event type to read.
         *
         * @return A span over all events of type T in the read buffer.
         */
        template<typename T>
        std::span<const T> read() const noexcept {

            const size_t index = TypeIndexGenerator::typeIndex<T>();

            if (buffers_.size() <= index || !buffers_[index]) {
                return {};
            }

            return static_cast<EventBuffer<T>*>(buffers_[index].get())->read();
        }

        /**
         * @brief Pre-allocates capacity for events of type T.
         *
         * @details Call during initialization to avoid allocations during gameplay.
         *
         * @tparam T The event type.
         *
         * @param size The number of events to reserve capacity for.
         */
        template<typename T>
        void reserve(size_t size) {
            getOrCreateBuffer<T>().reserve(size);
        }

        /**
         * @brief Gets or creates the buffer for event type T.
         *
         * @tparam T The event type.
         *
         * @return Reference to the EventBuffer for type T.
         */
        template<typename T>
        EventBuffer<T>& getOrCreateBuffer() {
            const size_t index = TypeIndexGenerator::typeIndex<T>();

            if (buffers_.size() <= index) {
                buffers_.resize(index + 1);
            }

            if (!buffers_[index]) {
                buffers_[index] = std::make_unique<EventBuffer<T>>();
            }

            return *static_cast<EventBuffer<T>*>(buffers_[index].get());
        }

        /**
         * @brief Swaps read and write buffers for all event types.
         *
         * @details Call once per frame at a consistent point (e.g., end of update phase).
         * After this call, events pushed in the current frame become readable.
         */
        void swapBuffers() {
            for (auto& buffer : buffers_) {
                if (buffer) {
                    buffer->swap();
                }
            }
        }

        /**
         * @brief Clears all read buffers.
         *
         * @details Typically not needed as `swapBuffers()` clears read buffers automatically.
         */
        void clearReadBuffers() {
            for (auto& buffer : buffers_) {
                if (buffer) {
                    buffer->clearReadBuffer();
                }
            }
        }

        /**
         * @brief Clears all write buffers without swapping.
         *
         * @details Use to discard events that were pushed but should not be processed.
         */
        void clearWriteBuffers() {
            for (auto& buffer : buffers_) {
                if (buffer) {
                    buffer->clearWriteBuffer();
                }

            }
        }
    };


}