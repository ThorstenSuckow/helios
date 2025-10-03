module;

#include <glad/gl.h>
#include <memory>

module helios.rendering.opengl.model;

import helios.rendering.model;
import :OpenGLMesh;

using namespace helios::rendering::model;

namespace helios::rendering::opengl::model {

        OpenGLNode::OpenGLNode(
            std::unique_ptr<Mesh> mesh,
            std::unique_ptr<Material> material) :
            Node(std::move(mesh), std::move(material)) {
        }

        void OpenGLNode::draw() const noexcept {
            auto& glMesh = dynamic_cast< OpenGLMesh&>(*mesh_);

            material_->use();

            glBindVertexArray(glMesh.vao());
            glDrawElements(GL_TRIANGLES,
                mesh_->meshData().indices().size() ,
                GL_UNSIGNED_INT, nullptr
            );
            glBindVertexArray(0);
        }

}