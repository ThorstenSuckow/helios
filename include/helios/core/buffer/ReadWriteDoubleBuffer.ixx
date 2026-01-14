/**
 * @file ReadWriteDoubleBuffer.ixx
 * @brief Type-safe double-buffered container for producer-consumer patterns.
 */
module;

#include <memory>
#include <span>
#include <vector>

export module helios.core.buffer.ReadWriteDoubleBuffer;

import helios.core.buffer.DoubleBuffer;
import helios.core.buffer.WriteBuffer;
import helios.core.buffer.ReadBuffer;


export namespace helios::core::buffer {
    /**
     * @class ReadWriteDoubleBuffer
     * @brief Type-safe double-buffered container for messages of type T.
     *
     * @details Implements a producer-consumer pattern where messages are written
     * to one buffer while consumers read from another. This decouples message
     * production from consumption and allows lock-free operation in single-threaded
     * game loops.
     *
     * @tparam T The message type stored in this buffer. Must be movable.
     */
    template<typename T>
    class ReadWriteDoubleBuffer : public DoubleBuffer {

        /**
         * @brief Buffer containing messages available for reading.
         */
        ReadBuffer<T> readBuffer_;

        /**
         * @brief Buffer where new messages are pushed.
         */
        WriteBuffer<T> writeBuffer_;


    public:

        /**
         * @brief Pre-allocates memory for both buffers.
         *
         * @param size The number of messages to reserve capacity for.
         */
        void reserve(size_t size) {
            readBuffer_.reserve(size);
            writeBuffer_.reserve(size);
        }

        /**
         * @brief Constructs and pushes a message to the write buffer.
         *
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to T's constructor.
         */
        template<typename... Args>
        void push(Args&&... args) {
            writeBuffer_.push(std::forward<Args>(args)...);
        }

        /**
         * @brief Returns a read-only view of messages in the read buffer.
         *
         * @return A span over all messages written in the previous frame.
         */
        std::span<const T> read() const noexcept {
            return readBuffer_.read();
        }

        /**
         * @brief Swaps read with write buffer.
         *
         * @details Swaps the read- with the write-buffer. The write buffer is empty after
         * this operation, while the read buffer contains the contents of the write buffer.
         */
        void swap() override {
            readBuffer_.clear().bufferData().swap(writeBuffer_.bufferData());
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
}