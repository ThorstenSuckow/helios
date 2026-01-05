/**
 * @file ReadBuffer.ixx
 * @brief Read-only buffer for the double-buffering pattern.
 */
module;

#include <span>
#include <vector>

export module helios.core.data.ReadBuffer;

export namespace helios::core::data {

    template <typename T>
    class ReadWriteDoubleBuffer{};

    /**
     * @class ReadBuffer
     * @brief Read-only buffer for consuming messages in a double-buffered system.
     *
     * @details ReadBuffer is the consumer-side of a double-buffered message system.
     * After a swap operation, this buffer contains messages that were pushed to the
     * corresponding WriteBuffer during the previous frame. Consumers iterate over
     * these messages via the read() method.
     *
     * The internal storage is only accessible to ReadWriteDoubleBuffer for swap operations.
     *
     * @tparam T The message type stored in the buffer.
     */
    template<typename T>
    class ReadBuffer {

        friend class ReadWriteDoubleBuffer<T>;

        /**
         * @brief Internal storage for buffered messages.
         */
        std::vector<T> bufferData_;

        /**
         * @brief Returns a mutable reference to the internal buffer.
         *
         * @details Only accessible to ReadWriteDoubleBuffer for swap operations.
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
         * @brief Pre-allocates memory for the specified number of messages.
         *
         * @param size The number of messages to reserve capacity for.
         *
         * @return Reference to this buffer for method chaining.
         */
        ReadBuffer& reserve(size_t size) {
            bufferData_.reserve(size);
            return *this;
        }

        /**
         * @brief Removes all messages from the buffer.
         *
         * @return Reference to this buffer for method chaining.
         */
        ReadBuffer& clear() {
            bufferData_.clear();
            return *this;
        }
    };

}
