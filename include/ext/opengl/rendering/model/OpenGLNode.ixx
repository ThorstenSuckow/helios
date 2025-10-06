module;


#include <memory>

export module helios.ext.opengl.rendering.model.OpenGLNode;

import helios.rendering.model;
import helios.ext.opengl.rendering.model.OpenGLMesh;

export namespace helios::ext::opengl::rendering::model {

    /**
     * An OpenGL specific implementation of a renderable Node.
     *
     * THis class provides the concrete logic for drawing a Mesh/Material
     * pair using the OpenGL API, overriding the draw() methode from the
     * base `Node`-class-
     */
    class OpenGLNode : public helios::rendering::model::Node {

    public:
        /**
         * @copydoc helios::rendering::model::Node::Node()
         */
        explicit OpenGLNode(std::unique_ptr<helios::rendering::model::Mesh> mesh,
            std::unique_ptr<helios::rendering::model::Material> material);


        /**
         * Renders this Node's Mesh and Material using OpenGL commands.
         */
        void draw() const noexcept override;
    };
}