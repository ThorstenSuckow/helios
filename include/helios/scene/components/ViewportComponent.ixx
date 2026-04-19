module;


export module helios.scene.components.ViewportComponent;

import helios.ecs.types.EntityHandle;
import helios.scene.types;
import helios.rendering.viewport;

using namespace helios::ecs::types;
using namespace helios::scene::types;
using namespace helios::rendering::viewport::types;
export namespace helios::scene::components {

    template<typename THandle>
    class ViewportComponent {

        SceneHandle sceneHandle_{};

        ViewportHandle viewportHandle_{};

        THandle cameraEntityHandle_{};

    public:

        explicit ViewportComponent(
            const ViewportHandle viewportHandle, const SceneHandle sceneHandle, const THandle cameraEntityHandle)
        : viewportHandle_(viewportHandle), sceneHandle_(sceneHandle), cameraEntityHandle_(cameraEntityHandle) {};

        [[nodiscard]] SceneHandle sceneHandle() const noexcept {
            return sceneHandle_;
        }

        [[nodiscard]] ViewportHandle viewportHandle() const noexcept {
            return viewportHandle_;
        }

        [[nodiscard]] THandle cameraEntityHandle() const noexcept {
            return cameraEntityHandle_;
        }


    };

}