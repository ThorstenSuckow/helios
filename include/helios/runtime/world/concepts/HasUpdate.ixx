/**
 * @file HasUpdate.ixx
 * @brief Concept requiring a per-frame update(UpdateContext&) method.
 */
module;

#include <concepts>

export module helios.runtime.world.concepts.HasUpdate;

import helios.runtime.world.UpdateContextFwd;

export namespace helios::runtime::concepts {

    /**a
     * @brief Requires that T provides a `void update(UpdateContext&)` method.
     *
     * @details This is the mandatory interface for concrete system classes.
     * The System wrapper enforces this concept at construction time.
     *
     * @tparam T The type to inspect.
     *
     * @see System
     */
    template<typename T>
    concept HasUpdate = requires(T& t, helios::runtime::world::UpdateContext& updateContext) {
        {t.update(updateContext) } -> std::same_as<void>;
    };

}


