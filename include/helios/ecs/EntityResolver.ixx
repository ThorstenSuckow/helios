/**
 * @file EntityResolver.ixx
 * @brief Lightweight callable for resolving EntityHandles to Entity wrappers.
 */
module;

#include <optional>

export module helios.ecs.EntityResolver;

import helios.ecs.EntityManager;
import helios.ecs.Entity;

import helios.ecs.types.EntityHandle;

using namespace helios::ecs::types;
export namespace helios::ecs {

    /**
     * @brief Callable that resolves an EntityHandle to an Entity facade.
     *
     * @details EntityResolver wraps an EntityManager pointer and provides
     * a validation-and-wrap operation: given an EntityHandle, it checks
     * validity via the EntityManager's versioned registry and returns a
     * lightweight Entity wrapper if the handle is still alive.
     *
     * Supports both function-call syntax and a named `find()` method.
     *
     * @tparam TEntityManager The concrete `EntityManager` specialisation
     *                        used for handle validation and Entity construction.
     *
     * @see EntityHandle
     * @see EntityManager
     * @see Entity
     */
    template<typename TEntityManager>
    struct EntityResolver {

        using Handle_type = TEntityManager::Handle_type;
        using Entity_type = Entity<TEntityManager>;
        
        /**
         * @brief Non-owning pointer to the EntityManager used for validation.
         */
        TEntityManager* const em;

        /**
         * @brief Resolves an EntityHandle to an Entity.
         *
         * @param handle The entity handle to resolve.
         *
         * @return An Entity if the handle is valid, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<Entity_type> operator()(const Handle_type handle) const noexcept {
            if (!em->isValid(handle)) {
                return std::nullopt;
            }
            return Entity_type(handle, em);
        }

        /**
         * @brief Resolves an EntityHandle to an Entity.
         *
         * @details Delegates to `operator()`.
         *
         * @param handle The entity handle to resolve.
         *
         * @return An Entity if the handle is valid, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<Entity_type> find(const Handle_type handle) const noexcept {
            return (*this)(handle);
        }



    };
}