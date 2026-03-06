/**
 * @file UiActionComponent.ixx
 * @brief Component for associating actions with UI elements.
 */
module;

export module helios.engine.modules.ui.widgets.components.UiActionComponent;

import helios.engine.modules.ui.widgets.types;

export namespace helios::engine::modules::ui::widgets::components {

    /**
     * @brief Associates an action identifier with a UI element.
     *
     * When a UI element (e.g., menu item) is activated, the associated
     * ActionId can be used to trigger the corresponding action.
     */
    class UiActionComponent {

        /**
         * @brief The action identifier associated with this UI element.
         */
        helios::engine::modules::ui::widgets::types::ActionId actionId_;

    public:

        /**
         * @brief Constructs the component with the given action ID.
         *
         * @param actionId The action identifier to associate.
         */
        explicit UiActionComponent(
            const helios::engine::modules::ui::widgets::types::ActionId actionId
        ) noexcept : actionId_{actionId} {}

        /**
         * @brief Returns the associated action identifier.
         *
         * @return The ActionId.
         */
        [[nodiscard]] helios::engine::modules::ui::widgets::types::ActionId actionId() const noexcept {
            return actionId_;
        }


    };



}