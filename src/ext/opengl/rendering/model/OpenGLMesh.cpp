module;

#include <glad/gl.h>
#include <memory>
#include <vector>
#include <stdexcept>

module helios.ext.opengl.rendering.model.OpenGLMesh;

import helios.rendering.Vertex;


using namespace helios::rendering::model;
using namespace helios::rendering;
using namespace helios::util;


namespace helios::ext::opengl::rendering::model {


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


    OpenGLMesh::OpenGLMesh(
        std::shared_ptr<const std::vector<helios::rendering::Vertex>> vertices,
        std::shared_ptr<const std::vector<unsigned int>> indices,
        std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
    )
       :
        Mesh(
            std::move(vertices),
            std::move(indices),
            std::move(meshConfig)
        ),
        vao_(generateGLVertexArray()),
        vbo_(generateGLBuffer()),
        ebo_(generateGLBuffer()) {

        if (!vertices_ || !indices_ || !meshConfig_) {
            const std::string msg = "Mesh constructor received a null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
        /**
         * @todo this should not be part of the constructor,
         * instead, lazy init in render pass, then reuse.
         */
        OpenGLMesh::init();
    }


    OpenGLMesh::OpenGLMesh(
    const helios::rendering::asset::shape::Shape& shape,
        std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
    )
       :
        Mesh(
            shape,
            std::move(meshConfig)
        ),
        vao_(generateGLVertexArray()),
        vbo_(generateGLBuffer()),
        ebo_(generateGLBuffer()) {

            if (!vertices_ || !indices_ || !meshConfig_) {
                const std::string msg = "Mesh constructor received a null shared pointer.";
                logger_.error(msg);
                throw std::invalid_argument(msg);
            }

            /**
             * @todo this should not be part of the constructor,
             * instead, lazy init in render pass, then reuse.
             */
            OpenGLMesh::init();
        }

    OpenGLMesh::~OpenGLMesh() {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
    }

    void OpenGLMesh::init() {

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        glBufferData(
            GL_ARRAY_BUFFER,
            vertices_->size() * sizeof(Vertex),
            &(*vertices_)[0],
            GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices_->size() * sizeof(unsigned int),
            &(*indices_)[0],
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
