/**
 * @file IsTypeIndexerLike.ixx
 * @brief Concept constraining compile-time type index providers.
 */
module;

#include <concepts>

export module helios.core.container.buffer.concepts.IsTypeIndexerLike;

export namespace helios::core::container::buffer::concepts {

    /**
     * @brief Constrains types that expose `typeIndex<T>() -> size_t`.
     *
     * @tparam T Candidate type indexer.
     */
    template<typename T>
    concept IsTypeIndexerLike = requires(T t)
    {
            {T::template typeIndex<int>()}->std::same_as<size_t>;
    };

}