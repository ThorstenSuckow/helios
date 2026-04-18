/**
 * @file HasSubmit.ixx
 * @brief Concept for types that can accept commands via submit().
 */
module;

#include <concepts>

export module helios.runtime.concepts.HasSubmit;

export namespace helios::runtime::concepts {

    /**
     * @brief Constrains types that can accept a command via a noexcept submit() method.
     *
     * @details Used by CommandHandlerRegistry to verify that a handler type
     * provides the required `submit(const CommandType&) noexcept -> bool`
     * interface before registering it as a command handler.
     *
     * @tparam OwningT The handler type.
     * @tparam CommandType The command type accepted by the handler.
     *
     * @see CommandHandlerRegistry
     */
    template<typename OwningT, typename CommandType>
    concept HasSubmit = requires(OwningT& owner, const CommandType& cmd) {
            { owner.submit(cmd) } noexcept -> std::same_as<bool>;
        };

}


