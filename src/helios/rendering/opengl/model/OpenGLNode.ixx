module;


#include <memory>

export module helios.rendering.opengl.model:OpenGLNode;

import helios.rendering.model;
import :OpenGLMesh;

using namespace helios::rendering::model;

export namespace helios::rendering::opengl::model {

    /**
     * OpenGL Node representative-
     */
    class OpenGLNode : public Node {

    public:

        explicit OpenGLNode(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material);

        void draw() const noexcept override;
    };
}