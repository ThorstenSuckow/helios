module;

#include <glad/gl.h>
#include <memory>

module helios.ext.opengl.rendering.model.OpenGLNode;

import helios.rendering.model;
import helios.ext.opengl.rendering.model.OpenGLMesh;

using namespace helios::rendering::model;

namespace helios::ext::opengl::rendering::model {

        OpenGLNode::OpenGLNode(
            std::unique_ptr<Mesh> mesh,
            std::unique_ptr<Material> material) :
            Node(std::move(mesh), std::move(material)) {
        }

        void OpenGLNode::draw() const noexcept {
            auto& glMesh = dynamic_cast< OpenGLMesh&>(*mesh_);

            material_->use();

            glBindVertexArray(glMesh.vao());
            glDrawElements(GL_LINE_LOOP,
                mesh_->meshData().indices().size() ,
                GL_UNSIGNED_INT, nullptr
            );
            glBindVertexArray(0);
        }

}