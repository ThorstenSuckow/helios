/**
 * @file PlayerComponent.ixx
 * @brief Marker component identifying the player entity.
 */
module;

export module helios.gameplay.matchstate.components.PlayerComponent;

export namespace helios::gameplay::matchstate::components {

    /**
     * @brief Component representing the player in a match.
     *
     * PlayerComponent is a marker component used to identify the player's
     * GameObject in the match. It can be used by systems to apply player-specific
     * logic, such as input handling, camera control, and UI updates.
     */
    template<typename THandle>
    class PlayerComponent {};

}