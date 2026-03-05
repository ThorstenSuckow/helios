/**
 * @file IsCommandHandler.ixx
 * @brief Concept constraining types that derive from CommandHandler.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsCommandHandler;

import helios.engine.common.concepts.HasTag;

export namespace helios::engine::runtime::messaging::command {
    class CommandHandler;
}


export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to types that derive from CommandHandler.
     *
     * @details Used by the ResourceRegistry to route registration to
     * the non-owning command-handler storage path (handlers are owned
     * by their respective Managers).
     *
     * @tparam T The type to constrain.
     *
     * @see CommandHandler
     * @see TypedCommandHandler
     * @see ResourceRegistry
     */
    template<typename T>
    concept IsCommandHandler = std::derived_from<T, helios::engine::runtime::messaging::command::CommandHandler>;

}
