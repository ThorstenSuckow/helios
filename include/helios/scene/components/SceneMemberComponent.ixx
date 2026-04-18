module;


export module helios.scene.components.SceneMemberComponent;

import helios.scene.types.SceneHandle;

using namespace helios::scene::types;

export namespace helios::scene::components {

    template<typename THandle>
    class SceneMemberComponent {

        SceneHandle sceneHandle_{};

    public:

        explicit SceneMemberComponent(const SceneHandle sceneId) : sceneHandle_(sceneId) {};

        [[nodiscard]] SceneHandle sceneHandle() const noexcept {
            return sceneHandle_;
        }


    };

}