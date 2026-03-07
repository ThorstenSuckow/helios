/**
 * @file IsCommandHandlerLike.ixx
 * @brief Concept constraining types that can handle specific commands.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsCommandHandlerLike;

export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to objects that provide a submit method for specified command types.
     *
     * @see CommandHandlerRegistry
     * @see ResourceRegistry
     */
    template<typename OwningT, typename... CommandType>
    concept IsCommandHandlerLike = (requires(OwningT& owner, const CommandType& cmd) {
          { owner.submit(cmd) } noexcept -> std::same_as<bool>;} && ...);
}
