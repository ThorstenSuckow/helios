/**
 * @file GameObjectFilterType.ixx
 * @brief Enumeration for filtering GameObjects in queries.
 */
module;

export module helios.engine.game.GameObjectFilterType;


export namespace helios::engine::game {

    /**
     * @brief Defines criteria for filtering GameObjects during iteration.
     *
     * @details
     * Used by `GameWorld::find()` and `GameObjectView` to select which entities
     * should be included in the results based on their active state.
     */
    enum class GameObjectFilterType {
        /**
         * @brief Include only active GameObjects.
         */
        Active,

        /**
         * @brief Include only inactive GameObjects.
         */
        Inactive,

        /**
         * @brief Include all GameObjects regardless of state.
         */
        All
    };

}