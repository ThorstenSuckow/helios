/**
 * @file Anchor.ixx
 * @brief Anchor points for UI element positioning.
 */
module;


export module helios.engine.modules.ui.layout.Anchor;


export namespace helios::engine::modules::ui::layout {

    /**
     * @brief Anchor points for UI element positioning.
     *
     * Defines reference points used to position UI elements relative to their
     * parent viewport or container.
     */
    enum class Anchor {

        /**
         * @brief Anchor to the center of the viewport.
         */
        Center,

        /**
         * @brief Anchor to the top-right corner of the viewport.
         */
        TopRight,

        /**
        * @brief Anchor to the bottom left corner of the viewport.
        */
       BottomLeft,

        /**
         * @brief Anchor to the top left corner of the viewport.
         */
        TopLeft,

        /**
         * @brief Sentinel value for iteration.
         */
        size_
    };


}