/**
 * @file ManagerRegistry.ixx
 * @brief Type-indexed registry for managing Manager instances.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>


export module helios.engine.runtime.world.ManagerRegistry;

import helios.core.container;
import helios.engine.runtime.world.Manager;
import helios.engine.common.concepts;
import helios.engine.runtime.world.types;

using namespace helios::core::container;
using namespace helios::engine::runtime::world::types;

export namespace helios::engine::runtime::world {

    /**
     * @brief Type alias for a ConceptModelRegistry specialized for Managers.
     *
     * @details Stores type-erased Manager wrappers indexed by ManagerTypeId,
     * providing O(1) lookup by concrete Manager type and insertion-order
     * iteration for deterministic init/flush/reset cycles.
     *
     * @see ConceptModelRegistry
     * @see Manager
     */
    using ManagerRegistry = ConceptModelRegistry<Manager, ManagerTypeId>;

}