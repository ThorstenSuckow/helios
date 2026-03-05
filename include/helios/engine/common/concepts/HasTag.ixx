/**
 * @file HasTag.ixx
 * @brief Concept for detecting a compile-time engine role tag on a type.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.HasTag;

export namespace helios::engine::common::concepts {

    /**
     * @brief Detects whether T declares a nested `EngineRoleTag` alias equal to Tag.
     *
     * @details Used by role-specific concepts (IsManager, IsSystem) to verify
     * that a type has been explicitly tagged for a particular engine role.
     * The tag must be declared as `using EngineRoleTag = Tag;` inside the class.
     *
     * @tparam T The type to inspect.
     * @tparam Tag The expected tag type (e.g. ManagerTag, SystemTag).
     *
     * @see ManagerTag
     * @see SystemTag
     */
    template<class T, class Tag>
    concept HasTag = requires { typename T::EngineRoleTag; } &&
                 std::same_as<typename T::EngineRoleTag, Tag>;

}