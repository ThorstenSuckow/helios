/**
 * @file SystemRegistry.ixx
 * @brief Type-indexed registry for managing System instances within a game loop pass.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>


export module helios.engine.runtime.world.SystemRegistry;

import helios.engine.core.data;
import helios.engine.runtime.world.System;
import helios.engine.common.concepts;


using namespace helios::engine::core::data;
using namespace helios::engine::common::concepts;

export namespace helios::engine::runtime::world {

    /**
     * @brief Type alias for a ConceptModelRegistry specialized for Systems.
     *
     * @details Stores type-erased System wrappers indexed by SystemTypeId,
     * providing O(1) lookup by concrete system type and insertion-order
     * iteration for deterministic update cycles within a game loop pass.
     *
     * @see ConceptModelRegistry
     * @see System
     */
    using SystemRegistry = ConceptModelRegistry<System, SystemTypeId>;

}