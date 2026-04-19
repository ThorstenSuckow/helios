/**
 * @file ReadWriteDoubleBufferFwd.ixx
 * @brief RFwd declaration for ReadWriteDoubleBufferFwd.
 */
module;

#include <span>
#include <vector>
#include <cstddef>

export module helios.core.container.buffer:ReadWriteDoubleBufferFwd;

export namespace helios::core::container::buffer {
    template <typename T>
    class ReadWriteDoubleBuffer;
}