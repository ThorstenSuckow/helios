/**
 * @file OpenGLMesh.ixx
 * @brief OpenGL-specific Mesh implementation and buffer management.
 */
module;

#include <glad/gl.h>
#include <memory>
#include <vector>
#include <stdexcept>
#include <string>

export module helios.ext.opengl.rendering.model.OpenGLMesh;

import helios.rendering.mesh.Mesh;
import helios.rendering.mesh.MeshConfig;
import helios.rendering.asset.shape.Shape;
import helios.rendering.Vertex;

export namespace helios::ext::opengl::rendering::model {

    /**
     * @brief Representative of an OpenGLMesh.
     * This class manages the OpenGL Vertex Array Object (VAO), the
     * Vertex Buffer Object (VBO) and Element Buffer Object (EBO) handles.
     * The raw mesh data is uploaded to the GPU, preparing it for subsequent
     * rendering commands / draw calls.
     */
    class OpenGLMesh final : public helios::rendering::mesh::Mesh {

    protected:


        /**
         * @brief Vertex Array Object handle.
         */
        const unsigned int vao_;


        /**
         * @brief Vertex Buffer Object handle.
         * Stores vertex attributes such as normals and texture coordinates.
         */
        const unsigned int vbo_;


        /**
         * @brief Element Buffer Object handle.
         * Stores the indices used for indexed drawing.
         */
        const unsigned int ebo_;


        /**
         * @brief Helper function for generating a Vertex Array Object identifier.
         *
         * @return Vertex Array Object handle.
         */
        static unsigned int generateGLVertexArray() noexcept {
            unsigned int vao;
            glGenVertexArrays(1, &vao);
            return vao;
        }


        /**
         * @brief Helper function for generating a Buffer Object identifier,
         * to be used with vertex buffer or element buffer.
         *
         * @return Vertex Array Object handle.
         */
        static unsigned int generateGLBuffer() noexcept {
            unsigned int vbo;
            glGenBuffers(1, &vbo);
            return vbo;
        }


        /**
         * @brief Initializes all buffer objects required by OpenGL from the provided mesh data.
         * The current implementation generates all VAO, VBO and EBO handles, loads the
         * vertex and index data to the GPU. It follows [Vri20, 162] in this regard.
         *
         * @see [Vri20, 162]
         */
        void init() override {
            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);

            glBufferData(
                GL_ARRAY_BUFFER,
                vertices_->size() * sizeof(helios::rendering::Vertex),
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
                GL_FALSE, sizeof(helios::rendering::Vertex), nullptr
            );

            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(helios::rendering::Vertex),
                reinterpret_cast<void*>(offsetof(helios::rendering::Vertex, normal))
            );

            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(helios::rendering::Vertex),
                reinterpret_cast<void*>(offsetof(helios::rendering::Vertex, texCoords))
            );

            glBindVertexArray(0);
        }

    public:

        /**
         * @brief Rule of three.
         * @see https://wikis.khronos.org/opengl/Common_Mistakes#RAII_and_hidden_destructor_calls
         * @see https://en.cppreference.com/w/cpp/language/rule_of_three.html
         */
        OpenGLMesh(const OpenGLMesh&) = delete;
        OpenGLMesh& operator=(const OpenGLMesh&) = delete;


        /**
         * @brief Constructs a new OpenGLMesh instance from raw mesh data.
         *
         * @param vertices A shared pointer to a vector of const Vertex
         * @param indices A shared pointer to a vector of indices
         * @param meshConfig A shared ptr to the const MeshConfig used with this Mesh.
         *
         * @throws std::invalid_argument if either "vertices", "indices" or meshConfig is a null shared pointer
         */
        explicit OpenGLMesh(
            std::shared_ptr<const std::vector<helios::rendering::Vertex>> vertices,
            std::shared_ptr<const std::vector<unsigned int>> indices,
            std::shared_ptr<const helios::rendering::mesh::MeshConfig> meshConfig
        ) :
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

        /**
         * @brief Creates a new OpenGLMesh instance from the specified Shape.
         *
         * @param shape A const reference to the Shape.
         * @param meshConfig A shared ptr to the const MeshConfig used with this OpenGLMesh.
         *
         * @throws std::invalid_argument if meshConfig is a null shared pointer, or if the
         * shape contained null data
         */
        explicit OpenGLMesh(
            const helios::rendering::asset::shape::Shape& shape,
            std::shared_ptr<const helios::rendering::mesh::MeshConfig> meshConfig
        ) :
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

        /**
         * @brief Frees allocated resources bv this instance.
         * If an instance of this class is destructed, `glDeleteBuffers`/`glDeleteVertexArrays`
         * is called to free the associated resources.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml
         *
         */
        ~OpenGLMesh() override {
            glDeleteVertexArrays(1, &vao_);
            glDeleteBuffers(1, &vbo_);
            glDeleteBuffers(1, &ebo_);
        }

        /**
         * @brief Returns the OpenGL Vertex Array Object Handle.
         *
         * @return VAO handle
         */
        [[nodiscard]] const unsigned int& vao() const noexcept {
            return vao_;
        }

        /**
         * @brief Returns the OpenGL Vertex Buffer Object handle.
         *
         * @return VBO handle
         */
        [[nodiscard]] const unsigned int& vbo() const noexcept {
            return vbo_;
        }

        /**
         * @brief Returns the OpenGL Element Buffer Object handle for indexed rendering.
         *
         * @return EBO handle
         */
        [[nodiscard]] const unsigned int& ebo() const noexcept {
            return ebo_;
        }

    };


}