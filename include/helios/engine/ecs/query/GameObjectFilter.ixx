/**
 * @file GameObjectFilter.ixx
 * @brief Enumeration for filtering GameObjects in queries.
 */
module;

#include <cstdint>

export module helios.engine.ecs.query.GameObjectFilter;

export namespace helios::engine::ecs::query {

    /**
     * @brief Defines criteria for filtering GameObjects during iteration.
     *
     * @details
     * Used by `GameWorld::find()` and `GameObjectView` to select which entities
     * should be included in the results based on their active state or component state.
     */
    enum class GameObjectFilter : uint8_t{
        /**
         * @brief No filtering applied.
         */
        None =  0,

        /**
         * @brief Include only active GameObjects.
         */
        Active = 1 << 0,

        /**
         * @brief Include only inactive GameObjects.
         */
        Inactive = 1 << 1,

        /**
         * @brief Include GameObjects with enabled components.
         */
        ComponentEnabled = 1 << 2,

        /**
         * @brief Include GameObjects with disabled components.
         */
        ComponentDisabled = 1 << 3,

        /**
         * @brief Include all GameObjects regardless of state.
         */
        All = Active | Inactive | ComponentEnabled | ComponentDisabled
    };

    /**
     * @brief Performs a bitwise AND operation between two GameObjectFilter values.
     *
     * @param a The first GameObjectFilter value.
     * @param b The second GameObjectFilter value.
     * @return A GameObjectFilter value representing the result of the AND operation.
     */
    constexpr GameObjectFilter operator &(GameObjectFilter a, GameObjectFilter b) noexcept {
        return static_cast<GameObjectFilter>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    /**
     * @brief Performs a bitwise OR operation between two GameObjectFilter values.
     *
     * @param a The first GameObjectFilter value.
     * @param b The second GameObjectFilter value.
     * @return A GameObjectFilter value representing the result of the OR operation.
     */
    constexpr GameObjectFilter operator |(GameObjectFilter a, GameObjectFilter b) noexcept {
        return static_cast<GameObjectFilter>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

}
