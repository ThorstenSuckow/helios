/**
 * @file GameObjectView.h
 * @brief Lazy range adapter for filtering GameObjects by component types.
 *
 * @note This file is intentionally a .h header instead of a .ixx module interface unit.
 *       Using this as a module interface unit causes Internal Compiler Errors (ICE)
 *       in MSVC (VS2022/VS2026) when structured bindings are used with the each()
 *       iterator (C1001 in p2/main.cpp). The workaround is to keep this as a
 *       traditional header included in the module's global fragment.
 */

#ifndef HELIOS_ENGINE_GAME_GAMEOBJECTVIEW_H
#define HELIOS_ENGINE_GAME_GAMEOBJECTVIEW_H

#include <type_traits>
#include <cassert>
#include <utility>
#include <tuple>

namespace helios::engine::game {
class GameObject;
} // namespace helios::engine::game


/**
 * @brief A lazy range adapter for filtering GameObjects by component types.
 *
 * Wraps the GameWorld's internal map and filters entities on-the-fly during iteration.
 * The range supports both const and non-const access depending on the underlying
 * map type, automatically selecting appropriate iterator and pointer types.
 *
 * **Example usage:**
 * ```cpp
 * // Range-based for loop returning GameObject pointers
 * for (auto* obj : gameWorld.find<Move2DComponent, SceneNodeComponent>()) {
 *     // obj is a GameObject* with guaranteed components
 * }
 *
 * // Using each() for direct component access via structured bindings
 * for (auto [entity, tc] : gameWorld.find<TransformComponent>().each()) {
 *     if (tc->isDirty()) tc->clearDirty();
 * }
 * ```
 *
 * @tparam MapT The map type (const or non-const unordered_map).
 * @tparam GameObjectT The GameObject type (for type flexibility).
 * @tparam Cs The component types to filter by. GameObjects must have ALL specified types.
 */
template<class MapT, class GameObjectT, class... Cs>
class GameObjectView {
private:
    /**
     * @brief Alias for the GameObject type.
     *
     * @note Parameterized via GameObjectT instead of hard-coded GameObject.
     */
    using Obj = GameObjectT;

    /**
     * @brief Removes const from MapT for iterator type deduction.
     */
    using RawMap = std::remove_const_t<MapT>;

    /**
     * @brief Iterator type selected based on map constness.
     *
     * @details Uses const_iterator for const maps, iterator otherwise.
     */
    using It = std::conditional_t<
        std::is_const_v<MapT>,
        typename RawMap::const_iterator,
        typename RawMap::iterator>;

    /**
     * @brief Pointer type to GameObject, respecting map constness.
     *
     * @details Returns const Obj* for const maps, Obj* otherwise.
     */
    using Ptr = std::conditional_t<std::is_const_v<MapT>, const Obj*, Obj*>;

    /**
     * @brief Normalizes component type by removing const/volatile/reference qualifiers.
     *
     * @tparam C The component type to normalize.
     *
     * @note Added to handle component types with cv-ref qualifiers.
     */
    template<class C>
    using Comp = std::remove_cvref_t<C>;

    /**
     * @brief Pointer type to component, respecting map constness.
     *
     * @tparam C The component type.
     *
     * @note Added for const-correct component pointer access.
     */
    template<class C>
    using CompPtr = std::conditional_t<std::is_const_v<MapT>, const Comp<C>*, Comp<C>*>;

    /**
     * @brief Pointer to the underlying map.
     *
     * @note Default-initialized to nullptr for safety.
     */
    MapT* objects_{nullptr};

    /**
     * @brief Checks if a GameObject has all required component types.
     *
     * @param obj Pointer to the GameObject to check.
     *
     * @return True if the GameObject has all component types Cs..., false otherwise.
     *
     * @note Uses Comp<Cs> to normalize types for the has<>() check.
     */
    [[nodiscard]] static bool matches(const Obj* obj) noexcept {
        return (obj->template has<Comp<Cs>>() && ...);
    }

public:
    /**
     * @brief Constructs a view over the given map.
     *
     * @param objects Reference to the map to iterate over.
     */
    explicit GameObjectView(MapT& objects) noexcept : objects_(&objects) {}

    /**
     * @brief Forward iterator for filtered GameObject traversal.
     *
     * @details Automatically skips GameObjects that don't match the component filter.
     * The iterator advances to the next matching entity on construction and increment.
     *
     * @note Simplified constructor signature; no longer stores MapT* internally.
     */
    class Iterator {
    private:
        /**
         * @brief Current position in the map.
         */
        It it_;   ///< Current position in the map.

        /**
         * @brief End iterator for bounds checking.
         */
        It end_;

        /**
         * @brief Advances to the next matching GameObject.
         *
         * @note Simplified implementation without storing objects_ reference.
         */
        void advanceToNextMatch() noexcept {
            while (it_ != end_ && !matches(it_->second.get())) {
                ++it_;
            }
        }

    public:
        /**
         * @brief Constructs an iterator at the given position.
         *
         * @param it Current iterator position.
         * @param end End iterator for bounds checking.
         */
        Iterator(It it, It end) noexcept : it_(it), end_(end) {
            advanceToNextMatch();
        }

        /**
         * @brief Dereferences to the current GameObject pointer.
         *
         * @return Pointer to the current GameObject.
         */
        [[nodiscard]] Ptr operator*() const noexcept {
            return static_cast<Ptr>(it_->second.get());
        }

        /**
         * @brief Advances to the next matching GameObject.
         *
         * @return Reference to this iterator after advancing.
         */
        Iterator& operator++() noexcept {
            ++it_;
            advanceToNextMatch();
            return *this;
        }

        /**
         * @brief Equality comparison.
         *
         * @param other The iterator to compare with.
         *
         * @return True if both iterators point to the same position.
         */
        [[nodiscard]] bool operator==(const Iterator& other) const noexcept {
            return it_ == other.it_;
        }

        /**
         * @brief Inequality comparison.
         *
         * @param other The iterator to compare with.
         *
         * @return True if iterators point to different positions.
         */
        [[nodiscard]] bool operator!=(const Iterator& other) const noexcept {
            return !(*this == other);
        }
    };

    /**
     * @brief Range adapter for iterating with direct component access.
     *
     * @details EachRange wraps a GameObjectView and provides an iterator that yields
     * tuples of (GameObject*, Component*...) for use with structured bindings.
     *
     * @note Yields pointer tuples (entity*, comp1*, comp2*...) instead of
     * reference wrappers.
     */
    class EachRange {
    private:
        /**
         * @brief Pointer to the base GameObjectView.
         *
         * @note Const pointer since EachRange does not modify the view.
         */
        const GameObjectView* baseRange_{nullptr};

    public:
        /**
         * @brief Constructs an EachRange over the given view.
         *
         * @param baseRange The GameObjectView to wrap.
         */
        explicit EachRange(const GameObjectView& baseRange) noexcept
            : baseRange_(&baseRange) {}

        /**
         * @brief Iterator yielding tuples of (GameObject*, Component*...).
         *
         * @details Each dereference returns a std::tuple containing:
         * - The GameObject pointer
         * - Pointers to each requested component type
         *
         * This enables structured binding syntax:
         * ```cpp
         * for (auto [obj, move, scene] : view.each()) { ... }
         * ```
         */
        class EachIterator {
        private:
            /**
             * @brief Underlying filtered iterator.
             */
            Iterator it_;

        public:
            /**
             * @brief Constructs an EachIterator wrapping a base Iterator.
             *
             * @param it The base iterator to wrap.
             */
            explicit EachIterator(Iterator it) noexcept : it_(it) {}

            /**
             * @brief Advances to the next element.
             *
             * @return Reference to this iterator after advancing.
             */
            EachIterator& operator++() noexcept {
                ++it_;
                return *this;
            }

            /**
             * @brief Equality comparison.
             *
             * @param other The iterator to compare with.
             *
             * @return True if both iterators point to the same position.
             */
            [[nodiscard]] bool operator==(const EachIterator& other) const noexcept {
                return it_ == other.it_;
            }

            /**
             * @brief Inequality comparison.
             *
             * @param other The iterator to compare with.
             *
             * @return True if iterators point to different positions.
             */
            [[nodiscard]] bool operator!=(const EachIterator& other) const noexcept {
                return !(*this == other);
            }

            /**
             * @brief Dereferences to a tuple of pointers.
             *
             * @return Tuple of (GameObject*, Component*...) with all requested components.
             *
             * @note Asserts that all components exist (guaranteed by the filter).
             * @note Returns pointer tuple instead of reference wrappers.
             */
            [[nodiscard]] auto operator*() const noexcept {
                Ptr obj = *it_;
                assert(((obj->template get<Comp<Cs>>() != nullptr) && ...));

                using Tup = std::tuple<Ptr, CompPtr<Cs>...>;
                return Tup{
                    obj,
                    static_cast<CompPtr<Cs>>(obj->template get<Comp<Cs>>())...
                };
            }
        };

        /**
         * @brief Returns iterator to the first element.
         *
         * @return EachIterator pointing to the first matching GameObject.
         */
        [[nodiscard]] EachIterator begin() const noexcept {
            return EachIterator(baseRange_->begin());
        }

        /**
         * @brief Returns iterator past the last element.
         *
         * @return EachIterator representing the end of the range.
         */
        [[nodiscard]] EachIterator end() const noexcept {
            return EachIterator(baseRange_->end());
        }
    };

    /**
     * @brief Returns a range for iterating with direct component access (const version).
     *
     * @return EachRange that yields (GameObject*, Component*...) tuples.
     */
    [[nodiscard]] EachRange each() const noexcept {
        return EachRange(*this);
    }

    /**
     * @brief Returns a range for iterating with direct component access.
     *
     * @return EachRange that yields (GameObject*, Component*...) tuples.
     */
    [[nodiscard]] EachRange each() noexcept {
        return EachRange(*this);
    }

    /**
     * @brief Returns iterator to the first matching GameObject.
     *
     * @return Iterator pointing to the first GameObject with all required components.
     *
     * @note Builds iterators directly from map iterators.
     */
    [[nodiscard]] Iterator begin() const noexcept {
        return Iterator(objects_->begin(), objects_->end());
    }

    /**
     * @brief Returns iterator past the last element.
     *
     * @return Iterator representing the end of the range.
     */
    [[nodiscard]] Iterator end() const noexcept {
        return Iterator(objects_->end(), objects_->end());
    }
};

#endif // HELIOS_ENGINE_GAME_GAMEOBJECTVIEW_H
