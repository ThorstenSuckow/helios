/**
 * @file EngineWorld.ixx
 * @brief Aggregate world that routes typed handles to their domain-specific TypedHandleWorld.
 */
module;

#include <coroutine>
#include <imgui_internal.h>
#include <tuple>
#include <type_traits>

export module helios.runtime.world.EngineWorld;

import helios.ecs;

import helios.runtime.world.GameObjectEntityManager;

import helios.platform.window.WindowEntityManager;
import helios.platform.window.concepts;
import helios.platform.window.types;

import helios.platform.environment.PlatformEntityManager;
import helios.platform.environment.types;

import helios.platform.concepts;
import helios.rendering.concepts;

import helios.rendering.shader.ShaderEntityManager;
import helios.rendering.material.MaterialEntityManager;
import helios.rendering.mesh.MeshEntityManager;

import helios.runtime.world.concepts.IsGameObjectHandle;
import helios.runtime.world.types.GameObjectHandle;

import helios.platform.window.concepts;
import helios.engine.common.concepts.IsResourceHandle;

import helios.rendering.concepts;

import helios.rendering.viewport.ViewportEntityManager;
import helios.rendering.framebuffer.FramebufferEntityManager;

import helios.core.TypedTupleCat;

using namespace helios::ecs;
using namespace helios::platform::environment;
using namespace helios::platform::environment::types;
using namespace helios::platform::concepts;
using namespace helios::platform::window;
using namespace helios::platform::window::types;
using namespace helios::platform::window::concepts;
using namespace helios::runtime::world::types;
using namespace helios::engine;
using namespace helios::runtime::world::concepts;
using namespace helios::engine::common::concepts;
using namespace helios::rendering::shader;
using namespace helios::rendering::material;
using namespace helios::rendering::mesh;
using namespace helios::rendering::concepts;
using namespace helios::rendering::framebuffer;
using namespace helios::rendering::viewport;


template<typename T>
inline constexpr bool typed_false = false;

export namespace helios::runtime::world {

    /**
     * @brief Typed world containing game-object entity managers.
     */
    using GameObjectWorld = TypedHandleWorld<GameObjectEntityManager>;

    /**
     * @brief Typed world containing render-resource entity managers.
     */
    using RenderResourceWorld = TypedHandleWorld<ShaderEntityManager, MaterialEntityManager, MeshEntityManager>;

    /**
     * @brief Typed world containing platform-related entity managers.
     */
    using PlatformWorld = TypedHandleWorld<WindowEntityManager, PlatformEntityManager>;

    /**
     * @brief Typed world containing render-target entity managers.
     */
    using RenderTargetWorld = TypedHandleWorld<FramebufferEntityManager, ViewportEntityManager>;

    /**
     * @brief Concatenated tuple of all entity-manager types used by `EngineWorld`.
     */
    using RegisteredEntityManagers = helios::core::TypedTupleCat<
        GameObjectWorld::EntityManager_types,
        RenderResourceWorld::EntityManager_types,
        PlatformWorld::EntityManager_types,
        RenderTargetWorld::EntityManager_types
    >::type;

    /**
     * @brief Aggregate runtime world for game objects, platform entities, and rendering domains.
     */
    class EngineWorld {

        GameObjectWorld gameObjectWorld_{};
        RenderResourceWorld renderResourceWorld_{};
        PlatformWorld platformWorld_{};
        RenderTargetWorld renderTargetWorld_{};

    public:

        [[nodiscard]] GameObjectWorld& gameObjectWorld() {
            return gameObjectWorld_;
        }

        [[nodiscard]] PlatformWorld& platformWorld() {
            return platformWorld_;
        }

        [[nodiscard]] RenderResourceWorld& renderResourceWorld() {
            return renderResourceWorld_;
        }

        [[nodiscard]] RenderTargetWorld& renderTargetWorld() {
            return renderTargetWorld_;
        }

        template<typename THandle>
        [[nodiscard]] auto clone(THandle source) noexcept {

            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.cloneEntity<THandle>(source);
            } else if constexpr(IsAnyPlatformHandle<THandle>){
                return platformWorld_.cloneEntity<THandle>(source);
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.cloneEntity<THandle>(source);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.cloneEntity<THandle>(source);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for cloning");
            }
        }


        template<typename THandle>
        [[nodiscard]] auto find(THandle handle) noexcept {
            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.findEntity<THandle>(handle);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.findEntity<THandle>(handle);
            } else  if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.findEntity<THandle>(handle);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.findEntity<THandle>(handle);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for searching");
            }
        }

        template<typename THandle>
        [[nodiscard]] auto add(typename THandle::StrongId_type strongId = typename THandle::StrongId_type{}) {
            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.addEntity<THandle>(strongId);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.addEntity<THandle>(strongId);
            }  else {
                static_assert(typed_false<THandle>, "Unsupported handle type for adding");
            }
        }


         template<typename THandle, typename... TComponents>
         [[nodiscard]] auto view() {
            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.template view<THandle, TComponents...>();
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.template view<THandle, TComponents...>();
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for viewing");
            }

        }

         template<typename THandle>
         [[nodiscard]] auto destroy(const THandle handle) {
            if constexpr(IsGameObjectHandle<THandle>) {
                return gameObjectWorld_.destroy<THandle>(handle);
            } else if constexpr(IsAnyPlatformHandle<THandle>) {
                return platformWorld_.destroy<THandle>(handle);
            } else  if constexpr(IsRenderResourceHandle<THandle>) {
                return renderResourceWorld_.destroy<THandle>(handle);
            } else if constexpr(IsRenderTargetHandle<THandle>) {
                return renderTargetWorld_.destroy<THandle>(handle);
            } else {
                static_assert(typed_false<THandle>, "Unsupported handle type for destroying");
            }

        }



    };

}