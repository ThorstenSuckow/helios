module;


#include <memory>

export module helios.ext.opengl.rendering.model.OpenGLNode;

import helios.rendering.model;
import helios.ext.opengl.rendering.model.OpenGLMesh;

using namespace helios::rendering::model;

export namespace helios::ext::opengl::rendering::model {

    /**
     * OpenGL Node representative-
     */
    class OpenGLNode : public Node {

    public:

        explicit OpenGLNode(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material);

        void draw() const noexcept override;
    };
}