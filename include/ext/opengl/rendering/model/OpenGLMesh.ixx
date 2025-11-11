/**
 * @file OpenGLMesh.ixx
 * @brief OpenGL-specific Mesh implementation and buffer management.
 */
module;

#include <memory>
#include <vector>

export module helios.ext.opengl.rendering.model.OpenGLMesh;

import helios.rendering.model.Mesh;
import helios.rendering.model.config.MeshConfig;
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
    class OpenGLMesh final : public helios::rendering::model::Mesh {

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
        static unsigned int generateGLVertexArray() noexcept;


        /**
         * @brief Helper function for generating a Buffer Object identifier,
         * to be used with vertex buffer or element buffer.
         *
         * @return Vertex Array Object handle.
         */
        static unsigned int generateGLBuffer() noexcept;


        /**
         * @brief Initializes all buffer objects required by OpenGL from the provided mesh data.
         * The current implementation generates all VAO, VBO and EBO handles, loads the
         * vertex and index data to the GPU. It follows [Vri20, 162] in this regard.
         *
         * @see [Vri20, 162]
         */
        void init() override;

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
            std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
        );

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
            std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
        );

        /**
         * @brief Frees allocated resources bv this instance.
         * If an instance of this class is destructed, `glDeleteBuffers`/`glDeleteVertexArrays`
         * is called to free the associated resources.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml
         *
         */
        ~OpenGLMesh() override;

        /**
         * @brief Returns the OpenGL Vertex Array Object Handle.
         *
         * @return VAO handle
         */
        [[nodiscard]] const unsigned int& vao() const noexcept;

        /**
         * @brief Returns the OpenGL Vertex Buffer Object handle.
         *
         * @return VBO handle
         */
        [[nodiscard]] const unsigned int& vbo() const noexcept;

        /**
         * @brief Returns the OpenGL Element Buffer Object handle for indexed rendering.
         *
         * @return EBO handle
         */
        [[nodiscard]] const unsigned int& ebo() const noexcept;

    };

}