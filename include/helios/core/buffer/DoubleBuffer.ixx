/**
 * @file DoubleBuffer.ixx
 * @brief Double-buffered, type-indexed message system for decoupled inter-system communication.
 */
module;

#include <memory>
#include <span>
#include <vector>

export module helios.core.buffer.DoubleBuffer;


export namespace helios::core::buffer {

    /**
     * @class DoubleBuffer
     * @brief Abstract base class for type-erased message buffers.
     *
     * @details Provides a common interface for buffer operations (swap, clear)
     * without exposing the concrete message type. Used internally by TypeIndexedDoubleBuffer
     * to manage heterogeneous message buffers in a single container.
     */
    class DoubleBuffer {

    public:
        DoubleBuffer() = default;

        virtual ~DoubleBuffer() = default;

        /**
         * @brief Swaps the read and write buffers.
         *
         * @details Clears the current read buffer, then exchanges it with the write buffer.
         * After this call, previously written messages become available for reading.
         */
        virtual void swap() = 0;

        /**
         * @brief Clears all messages from the read buffer.
         */
        virtual void clearReadBuffer() = 0;

        /**
         * @brief Clears all messages from the write buffer.
         */
        virtual void clearWriteBuffer() = 0;

    };
}
