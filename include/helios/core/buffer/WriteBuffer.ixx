/**
 * @file WriteBuffer.ixx
 * @brief Write-only buffer for the double-buffering pattern.
 */
module;

#include <vector>

export module helios.core.buffer.WriteBuffer;

import helios.core.buffer.ReadBuffer;

export namespace helios::core::buffer {

    template <typename T>
    class ReadWriteDoubleBuffer{};

    /**
     * @class WriteBuffer
     * @brief Write-only buffer for accumulating messages in a double-buffered system.
     *
     * @details WriteBuffer is the producer-side of a double-buffered message system.
     * Messages are pushed to this buffer during frame processing, then swapped with a
     * ReadBuffer at frame boundaries. This separation enables lock-free, single-threaded
     * producer-consumer patterns.
     *
     * The internal storage is only accessible to ReadWriteDoubleBuffer for swap operations.
     *
     * @tparam T The message type stored in the buffer. Must be move-constructible.
     */
    template<typename T>
    class WriteBuffer {

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
         * @brief Constructs and appends a message to the buffer.
         *
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to T's constructor.
         *
         * @return Reference to this buffer for method chaining.
         */
        template<typename... Args>
        WriteBuffer& push(Args&&... args) {
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
        WriteBuffer& reserve(size_t size) {
            bufferData_.reserve(size);
            return *this;
        }

        /**
         * @brief Removes all messages from the buffer.
         *
         * @return Reference to this buffer for method chaining.
         */
        WriteBuffer& clear() {
            bufferData_.clear();
            return *this;
        }
    };

}
