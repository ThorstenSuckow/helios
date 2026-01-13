/**
 * @file ReadWriteBuffer.ixx
 * @brief Combined read-write buffer for single-buffered access patterns.
 */
module;

#include <span>
#include <vector>

export module helios.core.buffer.ReadWriteBuffer;

import helios.core.buffer.Buffer;

export namespace helios::core::buffer {

    /**
     * @brief Combined read-write buffer with immediate visibility.
     *
     * @details Unlike the double-buffered ReadBuffer/WriteBuffer pair, ReadWriteBuffer
     * provides immediate access to pushed messages within the same frame. Messages
     * are visible to readers as soon as they are pushed.
     *
     * Use this when message producers and consumers operate in the same phase and
     * immediate visibility is required. For cross-phase communication, prefer
     * ReadWriteDoubleBuffer.
     *
     * @tparam T The message type stored in the buffer.
     */
    template<typename T>
    class ReadWriteBuffer : public helios::core::buffer::Buffer {

        /**
         * @brief Internal storage for buffered messages.
         */
        std::vector<T> bufferData_;

        /**
         * @brief Returns a mutable reference to the internal buffer.
         *
         * @return Reference to the internal vector.
         */
        [[nodiscard]] std::vector<T>& bufferData() {
            return bufferData_;
        }

    public:

        /**
         * @brief Returns a read-only view of all buffered messages.
         *
         * @return A span over all messages in the buffer.
         */
        std::span<const T> read() const noexcept {
            return bufferData_;
        }

        /**
         * @brief Constructs and appends a message to the buffer.
         *
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to T's constructor.
         *
         * @return Reference to this buffer for method chaining.
         */
        template<typename... Args>
        ReadWriteBuffer& push(Args&&... args) {
            bufferData_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        /**
         * @brief Pre-allocates memory for the specified number of messages.
         *
         * @param size The number of messages to reserve capacity for.
         *
         * @return Reference to this buffer for method chaining.
         */
        ReadWriteBuffer& reserve(size_t size) {
            bufferData_.reserve(size);
            return *this;
        }

        /**
         * @brief Removes all messages from the buffer.
         */
        void clear() override {
            bufferData_.clear();
        }
    };

}
