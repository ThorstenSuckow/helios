/**
 * @file TypeIndexer.ixx
 * @brief Utility for generating unique type indices at runtime.
 */
module;

#include <atomic>

export module helios.core.data.TypeIndexer;

export namespace helios::core::data {

    /**
     * @class TypeIndexer
     * @brief Utility class to generate unique type indices at runtime.
     *
     * @details This class provides a mechanism to generate and retrieve unique indices
     * for different types. It ensures that each type gets a distinct index
     * which remains constant throughout the program's execution.
     *
     * The indices are generated in a thread-safe manner using atomic operations.
     *
     * @tparam Group A tag type used to create separate index spaces. Different Group
     * types maintain independent counters, allowing the same type T to have different
     * indices in different contexts.
     *
     * Example usage:
     * ```cpp
     * class EventBus {
     *
     *     struct EventTypeGroup {};
     *
     *     using EventTypeIndexer = helios::core::TypeIndexer<EventTypeGroup>;
     *
     * public:
     *
     *     template<typename T>
     *     EventBuffer<T>& getOrAddBuffer() {
     *         const size_t index = EventTypeIndexer::typeIndex<T>();
     *         auto& buffer = lookupBufferAt(index);
     *         // ...
     *     }
     * };
     * ```
     */
    template<typename Group>
    class TypeIndexer {

        /**
         * @brief A static atomic counter used for generating unique type indices.
         */
        static inline std::atomic<size_t> counter_{0};

    public:

        /**
         * @brief Generates and returns a unique type index for a specific type.
         *
         * @details This method assigns a unique index to each type within
         * the scope of the associated TypeIndexer.
         *
         * @tparam T The type for which to retrieve the unique index.
         *
         * @return A unique index representing the specific type T within this Group.
         */
        template<typename T>
        static size_t typeIndex() {
            static const size_t typeIndex = counter_.fetch_add(1, std::memory_order::relaxed);
            return typeIndex;
        }

    };
}