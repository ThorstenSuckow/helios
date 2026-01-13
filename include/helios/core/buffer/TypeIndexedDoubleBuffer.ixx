/**
 * @file TypeIndexedDoubleBuffer.ixx
 * @brief Double-buffered, type-indexed message system for decoupled inter-system communication.
 */
module;

#include <memory>
#include <span>
#include <vector>

export module helios.core.buffer.TypeIndexedDoubleBuffer;

import helios.core.data.TypeIndexer;

import helios.core.buffer.DoubleBuffer;
import helios.core.buffer.WriteBuffer;
import helios.core.buffer.ReadBuffer;
import helios.core.buffer.ReadWriteDoubleBuffer;


export namespace helios::core::buffer {



    template<typename Indexer>
    /**
     * @class TypeIndexedDoubleBuffer
     * @brief Central hub for publishing and consuming typed messages.
     *
     * @details TypeIndexedDoubleBuffer provides a double-buffered, type-indexed message system
     * for decoupled inter-system communication. Systems push messages during their update phase,
     * then at frame boundaries `swapBuffers()` is called to make those messages available for reading.
     *
     * Each message type T gets its own dedicated ReadWriteDoubleBuffer, indexed via TypeIndexer
     * for O(1) access. This design enables efficient, allocation-friendly inter-system communication.
     *
     * Example usage:
     * ```cpp
     * // Define a message
     * struct CollisionMessage {
     *     EntityId a;
     *     EntityId b;
     *     vec3f contact;
     * };
     *
     * // In collision system
     * messageBuffer.push<CollisionMessage>(entityA, entityB, contactPoint);
     *
     * // At frame boundary
     * messageBuffer.swapBuffers();
     *
     * // In damage system (next phase or frame)
     * for (const auto& msg : messageBuffer.read<CollisionMessage>()) {
     *     applyDamage(msg.a, msg.b);
     * }
     * ```
     *
     * @tparam Indexer The TypeIndexer used for mapping message types to buffer indices.
     */
    class TypeIndexedDoubleBuffer {

        /**
         * @brief Type-erased storage for message buffers, indexed by type.
         */
        std::vector<std::unique_ptr<DoubleBuffer>> buffers_;

        /**
         * @brief Returns the buffer index for message type T.
         *
         * @tparam T The message type.
         *
         * @return The index assigned to type T by the Indexer.
         */
        template<typename T>
        static size_t index() {
            return Indexer::template typeIndex<T>();
        }

    public:

        /**
         * @brief Default destructor.
         */
        ~TypeIndexedDoubleBuffer() = default;

        /**
         * @brief Default constructor.
         */
        TypeIndexedDoubleBuffer() = default;

        /**
         * @brief Pushes a message of type T to the write buffer.
         *
         * @tparam T The message type.
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to T's constructor.
         */
        template<typename T, typename... Args>
        void push(Args&&... args) {
            getOrCreateBuffer<T>().push(std::forward<Args>(args)...);
        }

        /**
         * @brief Returns a read-only view of all messages of type T.
         *
         * @details Returns messages that were written before the last `swapBuffers()` call.
         * If no messages of type T exist, returns an empty span.
         *
         * @tparam T The message type to read.
         *
         * @return A span over all messages of type T in the read buffer.
         */
        template<typename T>
        std::span<const T> read() const noexcept {

            const size_t idx = index<T>();

            if (buffers_.size() <= idx || !buffers_[idx]) {
                return {};
            }

            return static_cast<ReadWriteDoubleBuffer<T>*>(buffers_[idx].get())->read();
        }

        /**
         * @brief Pre-allocates capacity for messages of type T.
         *
         * @details Call during initialization to avoid allocations during gameplay.
         *
         * @tparam T The message type.
         *
         * @param size The number of messages to reserve capacity for.
         */
        template<typename T>
        void reserve(size_t size) {
            getOrCreateBuffer<T>().reserve(size);
        }

        /**
         * @brief Gets or creates the buffer for message type T.
         *
         * @tparam T The message type.
         *
         * @return Reference to the ReadWriteDoubleBuffer for type T.
         */
        template<typename T>
        ReadWriteDoubleBuffer<T>& getOrCreateBuffer() {
            const size_t idx = index<T>();

            if (buffers_.size() <= idx) {
                buffers_.resize(idx + 1);
            }

            if (!buffers_[idx]) {
                buffers_[idx] = std::make_unique<ReadWriteDoubleBuffer<T>>();
            }

            return *static_cast<ReadWriteDoubleBuffer<T>*>(buffers_[idx].get());
        }

        /**
         * @brief Swaps read and write buffers for all message types.
         *
         * @details Call once per frame at a consistent point (e.g., end of update phase).
         * After this call, messages pushed in the current frame become readable.
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
         * @details Use to discard messages that were pushed but should not be processed.
         */
        void clearWriteBuffers() {
            for (auto& buffer : buffers_) {
                if (buffer) {
                    buffer->clearWriteBuffer();
                }

            }
        }

        /**
         * @brief Clears both read and write buffers for all message types.
         *
         * @details Completely resets the buffer state, discarding all messages.
         */
        void clearAll() {
            clearReadBuffers();
            clearWriteBuffers();
        }

        /**
         * @class WriteSink
         * @brief Lightweight handle for pushing messages to a TypeIndexedDoubleBuffer.
         *
         * @details WriteSink provides a focused interface for message producers that only
         * need write access. It holds a non-owning pointer to a TypeIndexedDoubleBuffer
         * and exposes only the push() operation.
         *
         * This class is useful for dependency injection where systems should be able to
         * publish messages without having access to buffer management operations like
         * swapBuffers() or clearReadBuffers().
         */
        class WriteSink {

            /**
             * @brief Non-owning pointer to the parent buffer.
             */
            TypeIndexedDoubleBuffer* db_ = nullptr;

        public:

            /**
             * @brief Constructs a WriteSink from a TypeIndexedDoubleBuffer reference.
             *
             * @param db The buffer to write to. Must remain valid for the lifetime of this sink.
             */
            explicit WriteSink(TypeIndexedDoubleBuffer& db) noexcept : db_(&db) {}

            /**
             * @brief Constructs and pushes a message of type E to the buffer.
             *
             * @tparam E The message type.
             * @tparam Args Constructor argument types for E.
             *
             * @param args Arguments forwarded to E's constructor.
             */
            template<typename E, typename... Args>
            void push(Args&&... args) noexcept {
                db_->template push <E>(std::forward<Args>(args)...);
            }
        };

        /**
         * @class ReadSource
         * @brief Lightweight handle for reading messages from a TypeIndexedDoubleBuffer.
         *
         * @details ReadSource provides a focused interface for message consumers that only
         * need read access. It holds a non-owning pointer to a TypeIndexedDoubleBuffer
         * and exposes only the read() operation.
         *
         * This class is useful for dependency injection where systems should be able to
         * consume messages without having access to buffer management operations.
         */
        class ReadSource {

            /**
             * @brief Non-owning pointer to the parent buffer.
             */
            TypeIndexedDoubleBuffer* db_ = nullptr;

        public:

            /**
             * @brief Constructs a ReadSource from a TypeIndexedDoubleBuffer reference.
             *
             * @param db The buffer to read from. Must remain valid for the lifetime of this source.
             */
            explicit ReadSource(TypeIndexedDoubleBuffer& db) noexcept : db_(&db) {}

            /**
             * @brief Returns a read-only view of messages of type E.
             *
             * @tparam E The message type to read.
             *
             * @return A span over all messages of type E in the read buffer.
             */
            template<typename E>
            std::span<const E> read() const noexcept {
                return db_->template read<E>();
            }
        };

        /**
         * @brief Creates a WriteSink handle for this buffer.
         *
         * @return A WriteSink that can push messages to this buffer.
         */
        [[nodiscard]] WriteSink writeSink() noexcept {
            return WriteSink(*this);
        }

        /**
         * @brief Creates a ReadSource handle for this buffer.
         *
         * @return A ReadSource that can read messages from this buffer.
         */
        [[nodiscard]] ReadSource readSource() noexcept {
            return ReadSource(*this);
        }

    };


}

