/**
 * @file IsViewportHandle.ixx
 * @brief Concept that constrains a type to the viewport handle type.
 */
module;

#include <concepts>

export module helios.rendering.viewport.concepts.IsViewportHandle;

import helios.rendering.viewport.types.ViewportHandle;

using namespace helios::rendering::viewport::types;
export namespace helios::rendering::viewport::concepts {

    /**
     * @brief Matches `ViewportHandle` exactly.
     *
     * @tparam T Type to test.
     */
    template<typename T>
    concept IsViewportHandle = std::same_as<T, ViewportHandle>;


}