module;

#include <glad/gl.h>
#include <memory>
#include <vector>
#include <stdexcept>

module helios.rendering.opengl.model;

import helios.rendering.model;
import helios.rendering.core.Vertex;


using namespace helios::rendering::model;
using namespace helios::rendering::core;
using namespace helios::util;


namespace helios::rendering::opengl::model {


    unsigned int OpenGLMesh::generateGLVertexArray() noexcept {
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        return vao;
    }


    unsigned int OpenGLMesh::generateGLBuffer() noexcept {
        unsigned int vbo;
        glGenBuffers(1, &vbo);
        return vbo;
    }


    OpenGLMesh::OpenGLMesh(std::shared_ptr<const MeshData> meshData)
       :
        Mesh(std::move(meshData)),
        vao_(generateGLVertexArray()),
        vbo_(generateGLBuffer()),
        ebo_(generateGLBuffer()) {
        OpenGLMesh::init();
    }

    OpenGLMesh::~OpenGLMesh() {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
    }

    void OpenGLMesh::init() {

        /**
         * @see [Vri20, 162]
         */
        const std::vector<Vertex>& vertices = meshData_->vertices();
        const std::vector<unsigned int>& indices = meshData_->indices();

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            &vertices[0],
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            &indices[0],
            GL_STATIC_DRAW
        );

        // vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 3, GL_FLOAT,
            GL_FALSE, sizeof(Vertex), nullptr
        );

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, normal))
        );

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, texCoords))
        );

        glBindVertexArray(0);
    }


    [[nodiscard]] const unsigned int& OpenGLMesh::vao() const noexcept {
        return vao_;
    }


    [[nodiscard]] const unsigned int& OpenGLMesh::vbo() const noexcept {
        return vbo_;
    }


    [[nodiscard]] const unsigned int& OpenGLMesh::ebo() const noexcept {
        return ebo_;
    }

};
