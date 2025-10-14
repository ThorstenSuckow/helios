module;

#include <memory>
#include <vector>

export module helios.scene.Snapshot;

import helios.scene.Camera;
import helios.scene.SceneNode;

import helios.math.types;

import helios.rendering.RenderQueue;
import helios.rendering.Renderable;

export namespace helios::scene {

    class Snapshot final {

    protected:
        const math::mat4f projectionMatrix_;
        const math::mat4f viewMatrix_;

        const std::vector<const helios::rendering::Renderable*> renderables_;
    public:
        // prevent copy
        Snapshot(const Snapshot&) = delete;
        Snapshot& operator=(const Snapshot&) = delete;

        ~Snapshot() = default;

        Snapshot(
            const math::mat4f& projectionMatrix,
            const math::mat4f& viewMatrix,
            std::vector<const helios::rendering::Renderable*> renderables
        ) :
            projectionMatrix_(projectionMatrix),
            viewMatrix_(viewMatrix),
            renderables_(std::move(renderables))
        {}
    };
}

