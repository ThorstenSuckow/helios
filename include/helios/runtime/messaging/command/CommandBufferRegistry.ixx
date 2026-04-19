/**
 * @file CommandBufferRegistry.ixx
 * @brief Type-indexed registry for CommandBuffer instances.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>


export module helios.runtime.messaging.command.CommandBufferRegistry;

import helios.core.container;
import helios.runtime.messaging.command.CommandBuffer;
import helios.runtime.messaging.command.types;
import helios.runtime.concepts;
import helios.runtime.world.types;

using namespace helios::core::container;
using namespace helios::runtime::messaging::command::types;

export namespace helios::runtime::messaging::command {

    /**
     * @brief Type alias for a ConceptModelRegistry specialized for CommandBuffers.
     *
     * @details Stores type-erased CommandBuffer wrappers indexed by
     * CommandBufferTypeId, providing O(1) lookup by concrete buffer type
     * and insertion-order iteration for deterministic flush cycles.
     *
     * @see ConceptModelRegistry
     * @see CommandBuffer
     * @see CommandBufferTypeId
     */
    using CommandBufferRegistry = ConceptModelRegistry<CommandBuffer, CommandBufferTypeId>;

}