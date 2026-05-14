/**
 * @file ViewportComponent.ixx
 * @brief Associates a scene and camera with a viewport handle.
 */
module;


export module helios.scene.components.ViewportComponent;

import helios.ecs.types.EntityHandle;
import helios.scene.types;
import helios.rendering.viewport.types;

using namespace helios::ecs::types;
using namespace helios::scene::types;
using namespace helios::rendering::viewport::types;
export namespace helios::scene::components {

    /**
     * @brief Component that groups viewport, scene, and camera bindings.
     *
     * @tparam THandle Camera entity handle type.
     */
    template<typename THandle>
    class ViewportComponent {

        SceneHandle sceneHandle_{};

        ViewportHandle viewportHandle_{};

        THandle cameraEntityHandle_{};

    public:

        /**
         * @brief Constructs the component from viewport, scene, and camera handles.
         *
         * @param viewportHandle Bound viewport handle.
         * @param sceneHandle Bound scene handle.
         * @param cameraEntityHandle Bound camera entity handle.
         */
        explicit ViewportComponent(
            const ViewportHandle viewportHandle, const SceneHandle sceneHandle, const THandle cameraEntityHandle)
        : viewportHandle_(viewportHandle), sceneHandle_(sceneHandle), cameraEntityHandle_(cameraEntityHandle) {};

        /** @brief Returns the bound scene handle. */
        [[nodiscard]] SceneHandle sceneHandle() const noexcept {
            return sceneHandle_;
        }

        /** @brief Returns the bound viewport handle. */
        [[nodiscard]] ViewportHandle viewportHandle() const noexcept {
            return viewportHandle_;
        }

        /** @brief Returns the bound camera entity handle. */
        [[nodiscard]] THandle cameraEntityHandle() const noexcept {
            return cameraEntityHandle_;
        }


    };

}