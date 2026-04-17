/**
 * @file IsEntityHandle.ixx
 * @brief Concept to detect ECS entity handle types.
 */
module;

#include <concepts>

export module helios.ecs.concepts.IsEntityHandle;
import helios.ecs.types.EntityHandle;
import helios.ecs.concepts.IsStrongIdLike;

using namespace helios::ecs::types;
export namespace helios::ecs::concepts {

    /**
     * @brief Constrains types that match the ECS EntityHandle shape.
     *
     * @tparam T Type to validate.
     */
    template<typename T>
    concept IsEntityHandle =
        requires{typename T::StrongId_type;} &&
        std::same_as<T, helios::ecs::types::EntityHandle<typename T::StrongId_type>> &&
        IsStrongIdLike<typename T::StrongId_type>;
}
