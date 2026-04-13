/**
 * @file TypedHandleWorld.ixx
 * @brief Multi-domain entity world with compile-time handle-to-manager dispatch.
 */
module;

#include <ostream>
#include <tuple>
#include <utility>
#include <vector>
#include <type_traits>
#include <optional>

export module helios.ecs.TypedHandleWorld;

import helios.ecs.View;
import helios.ecs.EntityResolver;
import helios.ecs.Entity;


/**
 * @brief Maps a handle type to the index of its owning EntityManager in a parameter pack.
 *
 * @details Recursively walks the `TEntityManagers...` pack and returns the
 * zero-based index of the first manager whose `Handle_type` matches `THandle`.
 * A `static_assert` fires if no match is found.
 *
 * @tparam THandle          The handle type to look up.
 * @tparam TEntityManagers  The EntityManager types to search.
 */
template<typename THandle, typename... TEntityManagers>
struct HandleToManager;

template<typename THandle, typename THead, typename ...TTail>
struct HandleToManager<THandle, THead, TTail...> {
    static constexpr size_t value = [] {
        if constexpr (std::is_same_v<typename THead::Handle_type, THandle>) {
            return size_t{0};
        } else {
            return size_t{1} + HandleToManager<THandle, TTail...>::value;
        }
    }();
};

template<typename THandle, typename THead>
struct HandleToManager<THandle, THead> {
    static_assert(
        std::is_same_v<typename THead::Handle_type, THandle>,
        "No EntityManager found for the given handle type."
    );
    static constexpr size_t value = 0;
};


template <typename THandle>
struct HandleToManager<THandle> {
    static_assert(sizeof(THandle) == 0, "No EntityManager found for the given handle type.");
};

export namespace helios::ecs {


    /**
     * @brief A multi-domain entity world that dispatches operations to the
     *        correct `EntityManager` based on the handle type.
     *
     * @details `TypedHandleWorld` holds a tuple of `EntityManager` instances,
     * each responsible for a different entity domain (identified by its handle
     * type). All public methods accept a handle-type template argument and
     * resolve the matching manager at **compile time** via `HandleToManager`.
     *
     * This enables a single world object to manage entities from multiple
     * independent registries — for example game entities, UI entities, and
     * audio entities — without runtime dispatch overhead.
     *
     * ## Domain-Specific Entity Managers
     *
     * Each `TEntityManagers` template argument is a fully configured
     * `EntityManager<THandle, TEntityRegistry, TCapacity>` specialisation.
     * The handle type (`THandle`) serves as the **domain key**: calling
     * `addEntity<MyHandle>()` automatically selects the manager whose
     * `Handle_type` is `MyHandle`.
     *
     * ## Usage
     *
     * ```cpp
     * // Define domain-specific handle types
     * using GameHandle = EntityHandle<GameStrongId>;
     * using UiHandle   = EntityHandle<UiStrongId>;
     *
     * // Configure managers
     * using GameEM = EntityManager<GameHandle, GameRegistry, 4096>;
     * using UiEM   = EntityManager<UiHandle,   UiRegistry,   512>;
     *
     * // Create the world
     * TypedHandleWorld<GameEM, UiEM> world;
     *
     * // Add entities — handle type selects the manager
     * auto player = world.addEntity<GameHandle>();
     * auto button = world.addEntity<UiHandle>();
     *
     * // Query a specific domain
     * for (auto [entity, transform, velocity] :
     *      world.view<GameHandle, TransformComponent, VelocityComponent>()) {
     *     // ...
     * }
     * ```
     *
     * ## Template Parameters
     *
     * @tparam TEntityManagers  One or more `EntityManager` specialisations.
     *                          Each must expose a unique `Handle_type` typedef.
     *
     * @see Entity
     * @see EntityManager
     * @see View
     * @see HandleToManager
     */
    template<typename... TEntityManagers>
    class TypedHandleWorld {

        std::tuple<TEntityManagers...> entityManagers_;


    public:

        /**
         * @brief Returns a reference to the EntityManager that owns handles
         *        of type `THandle`.
         *
         * @details Uses `HandleToManager` to resolve the tuple index at
         * compile time.  A `static_assert` fires if no manager matches.
         *
         * @tparam THandle The handle type identifying the target domain.
         *
         * @return Mutable reference to the matching EntityManager.
         */
        template<typename THandle>
        auto& entityManager() {
            constexpr size_t idx = HandleToManager<THandle, TEntityManagers...>::value;
            return std::get<idx>(entityManagers_);
        }

        /**
         * @brief Returns a const reference to the EntityManager that owns
         *        handles of type `THandle`.
         *
         * @tparam THandle The handle type identifying the target domain.
         *
         * @return Const reference to the matching EntityManager.
         */
        template<typename THandle>
        const auto& entityManager() const {
            constexpr size_t idx = HandleToManager<THandle, TEntityManagers...>::value;
            return std::get<idx>(entityManagers_);
        }


        /**
         * @brief Creates a new entity in the domain identified by `THandle`.
         *
         * @details Delegates to the matching `EntityManager::create()` and
         * wraps the result in an `Entity` facade.
         *
         * @tparam THandle The handle type identifying the target domain.
         *
         * @return An `Entity` wrapping the newly created handle.
         */
        template<typename THandle>
        [[nodiscard]] auto addEntity() {
            auto& em = entityManager<THandle>();

            auto handle = em.create();

            return Entity{handle, &em};
        }

        /**
         * @brief Looks up an existing entity by its handle.
         *
         * @details Validates the handle via the owning `EntityManager`. If
         * valid, returns an `Entity` facade; otherwise returns `std::nullopt`.
         *
         * @tparam THandle The handle type (and thus the domain) to search.
         *
         * @param handle The entity handle to resolve.
         *
         * @return `std::optional<Entity>` — engaged if the handle is valid.
         */
        template<typename THandle>
        [[nodiscard]] auto findEntity(THandle handle) {
            auto& em = entityManager<THandle>();

            using EM = std::remove_reference_t<decltype(em)>;
            using Entity_type = Entity<THandle, EM>;

            if (!em.isValid(handle)) {
                return std::optional<Entity_type>{std::nullopt};
            }

            return std::optional<Entity_type>{std::in_place, handle, &em};
        }


        /**
         * @brief Creates a `View` for iterating entities with specific components.
         *
         * @details The view operates on the `EntityManager` identified by
         * `THandle`.  The first component type in `TComponents` serves as the
         * lead set for iteration.
         *
         * @tparam THandle      The handle type selecting the domain.
         * @tparam TComponents   The component types to include in the view.
         *
         * @return A `View` over the matching EntityManager.
         *
         * @see View
         */
        template<typename THandle, typename ...TComponents>
        [[nodiscard]] auto view() {
            auto& em = entityManager<THandle>();
            using EM = std::remove_reference_t<decltype(em)>;
            return View<EM, TComponents...>(&em);
        }

    };

}

