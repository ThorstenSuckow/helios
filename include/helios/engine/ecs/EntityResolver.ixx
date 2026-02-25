/**
 * @file EntityResolver.ixx
 * @brief Lightweight callable for resolving EntityHandles to GameObjects.
 */
module;

#include <optional>

export module helios.engine.ecs.EntityResolver;

import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.EntityManager;
import helios.engine.ecs.GameObject;

export namespace helios::engine::ecs {

    /**
     * @brief Callable that resolves an EntityHandle to a GameObject.
     *
     * @details EntityResolver wraps an EntityManager pointer and provides
     * a validation-and-wrap operation: given an EntityHandle, it checks
     * validity via the EntityManager's versioned registry and returns a
     * lightweight GameObject wrapper if the handle is still alive.
     *
     * Supports both function-call syntax and a named `find()` method.
     *
     * @see EntityHandle
     * @see EntityManager
     * @see GameObject
     */
    struct EntityResolver {

        /**
         * @brief Non-owning pointer to the EntityManager used for validation.
         */
        helios::engine::ecs::EntityManager* em;

        /**
         * @brief Resolves an EntityHandle to a GameObject.
         *
         * @param handle The entity handle to resolve.
         *
         * @return A GameObject if the handle is valid, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<GameObject> operator()(const EntityHandle handle) const noexcept {
            if (!em->isValid(handle)) {
                return std::nullopt;
            }
            return GameObject(handle, em);
        }

        /**
         * @brief Resolves an EntityHandle to a GameObject.
         *
         * @details Delegates to `operator()`.
         *
         * @param handle The entity handle to resolve.
         *
         * @return A GameObject if the handle is valid, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<GameObject> find(const EntityHandle handle) const noexcept {
            return (*this)(handle);
        }
    };
}