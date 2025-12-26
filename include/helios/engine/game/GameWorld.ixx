/**
 * @file GameWorld.ixx
 * @brief Central registry for managing game entities.
 */
module;

#include <memory>
#include <unordered_map>
#include <type_traits>
#include <vector>
#include <cassert>
#include <utility>

export module helios.engine.game.GameWorld;

import helios.engine.game.GameObject;
import helios.util.Guid;
import helios.engine.game.UpdateContext;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.engine.game.System;
import helios.engine.game.Level;

namespace {

    /**
     * @brief Type alias for the internal GameObject storage map.
     */
    using Map = std::unordered_map<helios::util::Guid, std::unique_ptr<helios::engine::game::GameObject>>;

    /**
     * @brief A lazy range adapter for filtering GameObjects by component types.
     *
     * FindRange provides a lazy iteration mechanism for querying GameObjects
     * that possess a specific set of components. It wraps the GameWorld's internal map
     * and filters entities on-the-fly during iteration.
     *
     * The range supports both const and non-const access depending on the underlying
     * map type, automatically selecting appropriate iterator and pointer types.
     *
     * Example usage:
     * ```cpp
     * // Iterate over all GameObjects with Move2DComponent and SceneNodeComponent
     * for (auto* obj : gameWorld.find<Move2DComponent, SceneNodeComponent>()) {
     *     // obj is a pointer to a matching GameObject
     * }
     *
     * // Alternative: use each() for direct component access
     * gameWorld.find<Move2DComponent>().each([](GameObject* obj, Move2DComponent& move) {
     *     move.setVelocity({1.0f, 0.0f, 0.0f});
     * });
     * ```
     *
     * @tparam MapT The map type (const or non-const).
     * @tparam Cs The component types to filter by.
     */
    template <class MapT, class... Cs>
    class FindRange {

    private:

        using Obj = helios::engine::game::GameObject;
        using RawMap = std::remove_const_t<MapT>;

        /**
         * @brief Iterator type, const_iterator for const maps, iterator otherwise.
         */
        using It = std::conditional_t<std::is_const_v<MapT>,
                    typename RawMap::const_iterator,
                    typename RawMap::iterator>;

        /**
         * @brief Pointer type, const pointer for const maps, non-const otherwise.
         */
        using Ptr = std::conditional_t<std::is_const_v<MapT>, const Obj*, Obj*>;

        /**
         * @brief Non-owning pointer to the underlying GameObject map.
         */
        MapT* objects_;

        /**
         * @brief Checks if a GameObject has all required component types.
         *
         * @param obj The GameObject to check.
         *
         * @return True if the object has all components Cs..., false otherwise.
         */
        static bool matches(const Obj* obj) {
            return (obj->template has<std::remove_cvref_t<Cs>>() && ...);
        }

    public:

        /**
         * @brief Constructs a FindRange for the given GameObject map.
         *
         * @param objects Reference to the map to iterate over.
         */
        explicit FindRange(MapT& objects) : objects_(&objects) {}

        /**
         * @brief Forward iterator for filtered GameObject traversal.
         *
         * This iterator skips GameObjects that do not match the component filter,
         * advancing to the next matching entity automatically.
         */
        class Iterator {

        private:
            MapT* objects_{nullptr};
            It it_;
            It end_;

            /**
             * @brief Advances the iterator to the next matching GameObject.
             *
             * @details Skips entries that do not have all required components.
             */
            void query_next() {
                while (it_ != end_) {
                    auto* obj = it_->second.get();

                    if (matches(obj)) {
                        return;
                    }

                    ++it_;
                }
            }

        public:

            /**
             * @brief Constructs an iterator at the given position.
             *
             * @param objects Pointer to the underlying map.
             * @param it Starting iterator position.
             * @param end End iterator position.
             */
            Iterator(MapT* objects, It it, It end) : objects_(objects), it_(it), end_(end) {
                query_next();
            }

            /**
             * @brief Dereferences the iterator to get the current GameObject.
             *
             * @return Pointer to the current matching GameObject.
             */
            Ptr operator*() const {
                return static_cast<Ptr>(it_->second.get());
            }

            /**
             * @brief Advances the iterator to the next matching GameObject.
             *
             * @return Reference to this iterator.
             */
            Iterator& operator++() {
                ++it_;
                query_next();
                return *this;
            }

            /**
             * @brief Compares two iterators for equality.
             *
             * @param other The iterator to compare with.
             *
             * @return True if both iterators point to the same position.
             */
            bool operator==(const Iterator& other) const {
                return objects_ == other.objects_ && it_ == other.it_;
            }

            /**
             * @brief Compares two iterators for inequality.
             *
             * @param other The iterator to compare with.
             *
             * @return True if iterators point to different positions.
             */
            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

        };

        /*
         // the following code does not compile when each() is invoked and tuples are considered
         // as return types: auto [entity, tc] = gameWorld->_find<TransformComponent>().each();
         // this leads to a ICE with VSC 17
         class EachRange {

            FindRange* baseRange_;

        public:
            explicit EachRange(FindRange& baseRange) : baseRange_(&baseRange) {}

            class EachIterator {
                Iterator it_;

            public:
                explicit EachIterator(Iterator it) : it_(it) {}

                EachIterator& operator++() {
                    ++it_;
                    return *this;
                }

                bool operator==(const EachIterator& other) const {
                    return it_ == other.it_;
                }

                bool operator!=(const EachIterator& other) const {
                    return !(*this == other);
                }

                auto operator*() const {
                    auto* obj = *it_;
                    assert(((obj->template get<std::remove_cvref_t<Cs>>() != nullptr) && ...));
                    return std::tuple{
                        obj,
                        std::ref(*obj->template get<std::remove_cvref_t<Cs>>())...
                    };
                }
            };

            EachIterator begin() const {
                return EachIterator(baseRange_->begin());
            }

            EachIterator end() const {
                return EachIterator(baseRange_->end());
            }
        };

        EachRange each() const {
            return EachRange(const_cast<FindRange&>(*this));
        }

        EachRange each()  {
            return EachRange(*this);
        }*/

        /**
         * @brief Invokes a callback for each matching GameObject with its components.
         *
         * @details This method iterates over all GameObjects that have all specified component types
         * and invokes the provided callback with a pointer to the GameObject and references
         * to each requested component.
         *
         * Example:
         * ```cpp
         * gameWorld.find<Move2DComponent, SceneNodeComponent>().each(
         *     [](GameObject* obj, Move2DComponent& move, SceneNodeComponent& scene) {
         *         move.setVelocity({1.0f, 0.0f, 0.0f});
         *     }
         * );
         * ```
         *
         * @tparam Func Callable type accepting (GameObject*, Cs&...).
         *
         * @param func The callback to invoke for each matching entity.
         *
         * @note Components are passed by reference. The assertion ensures all components exist.
         */
        template <typename Func>

        void each(Func&& func) const {
            for (auto& [id, uptr] : *objects_) {

                auto* raw = uptr.get();
                if (!matches(raw)) {
                    continue;
                }

                Ptr obj = static_cast<Ptr>(raw);

                assert(((raw->template get<std::remove_cvref_t<Cs>>() != nullptr) && ...) && "Unexpected nullptr in each.");

                std::forward<Func>(func)(
                    obj,
                    (*raw->template get<std::remove_cvref_t<Cs>>())...
                );
            }
        }

        /**
         * @brief Returns an iterator to the first matching GameObject.
         *
         * @return Iterator pointing to the first GameObject with all required components,
         *         or end() if no match exists.
         */
        Iterator begin() const {
            return Iterator(objects_, objects_->begin(), objects_->end());
        }

        /**
         * @brief Returns an iterator past the last GameObject.
         *
         * @return Iterator representing the end of the range.
         */
        Iterator end() const {
            return Iterator(objects_, objects_->end(), objects_->end());
        }
    };

}

#define HELIOS_LOG_SCOPE "helios::engine::game::GameWorld"
export namespace helios::engine::game {



    /**
     * @brief Central registry for managing game entities, systems, and the active level.
     *
     * @details
     * The GameWorld is the root container for the game state. It manages the lifecycle
     * of GameObjects, executes Systems, and holds the current Level.
     *
     * Key responsibilities:
     * - **Entity Management:** Owns all GameObjects and provides lookup by Guid.
     * - **System Execution:** Manages and updates registered Systems.
     * - **Level Management:** Holds the active Level instance.
     * - **Update Loop:** Orchestrates the frame update by updating GameObjects and then Systems.
     */
    class GameWorld {

    protected:
        /**
         * @brief Hash map storing all active GameObjects, indexed by their Guid.
         *
         * @details Uses std::unordered_map for O(1) average-case lookup performance.
         * The map owns all GameObjects via std::unique_ptr.
         */
        std::unordered_map<helios::util::Guid, std::unique_ptr<GameObject>> gameObjects_;

        /**
         * @brief The logger used with this GameWorld instance.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

        /**
         * @brief Vector storing all registered Systems.
         *
         * @details Systems are updated in order of registration after all GameObjects
         *          have been updated.
         */
        std::vector<std::unique_ptr<System>> systems_;

        /**
         * @brief The current level loaded in the game world.
         *
         * @details Can be null if no level is currently active.
         */
        std::unique_ptr<helios::engine::game::Level> level_ = nullptr;

    public:


        explicit GameWorld() = default;


        /**
         * @brief Sets the current level for the game world.
         *
         * @param level Unique pointer to the Level instance. Ownership is transferred to the GameWorld.
         */
        void setLevel(std::unique_ptr<helios::engine::game::Level> level) noexcept {
            level_ = std::move(level);
        }

        /**
         * @brief Checks if a level is currently loaded.
         *
         * @return True if a level is set, false otherwise.
         */
        [[nodiscard]] bool hasLevel() const noexcept{
            return level_ != nullptr;
        }

        /**
         * @brief Retrieves the currently loaded level.
         *
         * @return Reference to the active Level.
         *
         * @warning Calling this method when hasLevel() returns false results in undefined behavior.
         */
        [[nodiscard]] const helios::engine::game::Level& level() const noexcept{
            return *level_;
        }

        /**
         * @brief Adds a System to the GameWorld.
         *
         * @details Creates a System of the specified type with the given constructor arguments,
         *          invokes its `onAdd()` callback, and registers it with this GameWorld.
         *          Systems are updated each frame after GameObjects.
         *
         * @tparam T The System type to add. Must derive from System.
         * @tparam Args Constructor argument types.
         * @param args Arguments forwarded to the System constructor.
         *
         * @return Reference to the newly added System.
         *
         * @note The GameWorld takes ownership of the System.
         */
        template<typename T, typename... Args>
        T& add(Args&&... args) {
            auto system_ptr = std::make_unique<T>(std::forward<Args>(args)...);
            T* raw_ptr = system_ptr.get();

            system_ptr->onAdd(this);
            systems_.push_back(std::move(system_ptr));

            return *raw_ptr;
        }

        /**
         * @brief Retrieves a System of the specified type.
         *
         * @details Searches through all registered Systems using dynamic_cast
         *          to find a matching type.
         *
         * @tparam T The System type to retrieve. Must derive from System.
         *
         * @return Pointer to the System if found, nullptr otherwise.
         *
         * @note Uses dynamic_cast internally; consider type-based registry for
         *       performance-critical paths.
         */
        template<typename T>
        [[nodiscard]] T* get() const {
            for (const auto& system : systems_) {
                if (auto* sys = dynamic_cast<T*>(system.get())) {
                    return sys;
                }
            }
            return nullptr;
        }

        /**
         * @brief Checks if a System of the specified type is registered.
         *
         * @tparam T The System type to check for. Must derive from System.
         *
         * @return True if a System of type T exists, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool has() const {
            return get<T>() != nullptr;
        }

        /**
         * @brief Updates the game world state for the current frame.
         *
         * @details
         * This method performs the following steps:
         * 1. Updates all active GameObjects (invoking their `update()` method).
         * 2. Updates all registered Systems (invoking their `update()` method).
         *
         * @param updateContext Context containing deltaTime, input snapshot, game world and command buffer.
         *
         * @note The order of GameObject updates is not guaranteed. Systems are updated in registration order.
         */
        void update(helios::engine::game::UpdateContext& updateContext) const noexcept;

        /**
         * @brief Adds a GameObject to the world and transfers ownership.
         *
         * @param gameObject The GameObject to add. Ownership is transferred to the GameWorld.
         *
         * @return Pointer to the added GameObject, or nullptr if addition failed.
         *
         * @retval GameObject* Non-owning pointer to the added object (valid until removed)
         * @retval nullptr If gameObject was null or a GameObject with the same Guid already exists
         *
         * @note The GameWorld takes ownership of the GameObject. The returned pointer remains
         *       valid until the GameObject is removed from the world.
         * @note Attempting to add a nullptr or a GameObject with a duplicate Guid will fail,
         *       return nullptr, and log a warning.
         */
        [[nodiscard]] helios::engine::game::GameObject* addGameObject(std::unique_ptr<GameObject> gameObject);

        /**
         * @brief Finds a GameObject by its unique identifier.
         *
         * @param guid The unique identifier of the GameObject to find.
         *
         * @return Pointer to the GameObject if found, nullptr otherwise.
         *
         * @retval GameObject* Non-owning pointer to the found object
         * @retval nullptr If no GameObject with the specified Guid exists
         *
         * @note This is the non-const overload. Use the const overload for read-only access.
         */
        [[nodiscard]] helios::engine::game::GameObject* find(const helios::util::Guid& guid);

        /**
         * @brief Finds all GameObjects that have the specified component types.
         *
         * @details
         * Returns a lazy range that filters GameObjects on-the-fly during iteration.
         * Only GameObjects possessing all specified component types are yielded.
         *
         * Example usage:
         * ```cpp
         * // Range-based for loop
         * for (auto* obj : gameWorld.find<Move2DComponent, SceneNodeComponent>()) {
         *     auto* move = obj->get<Move2DComponent>();
         *     move->setVelocity({1.0f, 0.0f, 0.0f});
         * }
         *
         * // Using each() for direct component access
         * gameWorld.find<Move2DComponent>().each(
         *     [](GameObject* obj, Move2DComponent& move) {
         *         move.setVelocity({1.0f, 0.0f, 0.0f});
         *     }
         * );
         * ```
         *
         * @tparam Cs The component types to filter by. GameObjects must have all types.
         *
         * @return A FindRange object that can be iterated over.
         */
        template<class... Cs>
        [[nodiscard]] auto find() {
            return FindRange<Map, Cs...>(gameObjects_);
        }

        /**
         * @brief Finds all GameObjects that have the specified component types (const overload).
         *
         * @details Const version of the component-based find. Returns const pointers to GameObjects.
         *
         * @tparam Cs The component types to filter by. GameObjects must have all types.
         *
         * @return A const FindRange object that can be iterated over.
         */
        template<class... Cs>
        [[nodiscard]] auto find() const {
            return FindRange<const Map, Cs...>(gameObjects_);
        }

        /**
         * @brief Finds a GameObject by its unique identifier (const overload).
         *
         * @param guid The unique identifier of the GameObject to find.
         *
         * @return Const pointer to the GameObject if found, nullptr otherwise.
         *
         * @retval const GameObject* Non-owning const pointer to the found object
         * @retval nullptr If no GameObject with the specified Guid exists
         *
         * @note This overload is used when the GameWorld is accessed via const reference.
         */
        [[nodiscard]] const helios::engine::game::GameObject* find(const helios::util::Guid& guid) const;

        /**
         * @brief Removes a GameObject from the world and transfers ownership to the caller.
         *
         * @param gameObject Reference to the GameObject to remove.
         *
         * @return Unique pointer to the removed GameObject, or nullptr if not found.
         *
         * @retval std::unique_ptr<GameObject> Ownership of the removed object
         * @retval nullptr If the GameObject was not found in the world
         *
         * @note After successful removal, all non-owning pointers to the GameObject
         *       become invalid unless the caller maintains the returned unique_ptr.
         * @note Attempting to remove a GameObject that doesn't exist returns nullptr
         *       and logs a warning.
         */
        [[nodiscard]] std::unique_ptr<helios::engine::game::GameObject> removeGameObject(const GameObject& gameObject);

        /**
         * @brief Retrieves a const ref to the map of all active GameObjects.
         *
         * @return A const reference to the internal map of GameObjects, indexed by Guid.
         *
         * @warning Modifying the map directly (e.g. adding/removing elements) bypasses
         *          GameWorld's management logic and should be avoided. Use addGameObject()
         *          and removeGameObject() instead.
         */
        [[nodiscard]] const std::unordered_map<helios::util::Guid, std::unique_ptr<GameObject>>& gameObjects() const noexcept {
            return gameObjects_;
        }
    };

}

