/**
 * @file IsCommandBuffer.ixx
 * @brief Concept constraining types that derive from CommandBuffer.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsCommandBuffer;

import helios.engine.common.concepts.HasTag;

export namespace helios::engine::runtime::messaging::command {
    class CommandBuffer;
}


export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to types that derive from CommandBuffer.
     *
     * @details Used by the ResourceRegistry to route registration to
     * the command-buffer storage path, which also tracks buffers in a
     * flat iteration list for batch flushing.
     *
     * @tparam T The type to constrain.
     *
     * @see CommandBuffer
     * @see ResourceRegistry
     */
    template<class T>
    concept IsCommandBuffer = std::derived_from<T, helios::engine::runtime::messaging::command::CommandBuffer>;
}
