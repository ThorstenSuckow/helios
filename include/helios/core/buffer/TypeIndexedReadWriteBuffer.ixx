/**
 * @file TypeIndexedReadWriteBuffer.ixx
 * @brief Type-indexed single-buffered message system for immediate-access patterns.
 */
module;

#include <memory>
#include <span>
#include <vector>

export module helios.core.buffer.TypeIndexedReadWriteBuffer;

import helios.core.data.TypeIndexer;

import helios.core.buffer.Buffer;
import helios.core.buffer.ReadWriteBuffer;


export namespace helios::core::buffer {

    /**
     * @brief Type-indexed container for immediate-access message buffers.
     *
     * @details Unlike TypeIndexedDoubleBuffer, this container provides immediate
     * visibility of pushed messages within the same frame. Each message type T
     * gets its own dedicated ReadWriteBuffer, indexed via TypeIndexer for O(1) access.
     *
     * Use this when message producers and consumers operate in the same phase.
     * For cross-phase communication with frame-boundary swapping, prefer
     * TypeIndexedDoubleBuffer.
     *
     * @tparam Indexer The TypeIndexer used for mapping message types to buffer indices.
     */
    template<typename Indexer>

    class TypeIndexedReadWriteBuffer {

        /**
         * @brief Type-erased storage for message buffers, indexed by type.
         */
        std::vector<std::unique_ptr<Buffer>> buffers_;

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
        ~TypeIndexedReadWriteBuffer() = default;

        /**
         * @brief Default constructor.
         */
        TypeIndexedReadWriteBuffer() = default;

        /**
         * @brief Pushes a message of type T to the buffer.
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
         * @details Returns messages that were written to the buffer.
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

            return static_cast<ReadWriteBuffer<T>*>(buffers_[idx].get())->read();
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
         * @return Reference to the ReadWriteBuffer for type T.
         */
        template<typename T>
        ReadWriteBuffer<T>& getOrCreateBuffer() {
            const size_t idx = index<T>();

            if (buffers_.size() <= idx) {
                buffers_.resize(idx + 1);
            }

            if (!buffers_[idx]) {
                buffers_[idx] = std::make_unique<ReadWriteBuffer<T>>();
            }

            return *static_cast<ReadWriteBuffer<T>*>(buffers_[idx].get());
        }


        /**
         * @brief Clears all buffers.
         */
        void clear() {
            for (auto& buffer : buffers_) {
                if (buffer) {
                    buffer->clear();
                }
            }
        }

    };


}

