module;

#include <memory>

export module helios.ext.opengl.rendering.model.OpenGLMesh;

import helios.rendering.model.Mesh;
import helios.rendering.model.MeshData;

export namespace helios::ext::opengl::rendering::model {

    /**
     * @brief Representative of an OpenGLMesh.
     * This class manages the OpenGL Vertex Array Object (VAO), the
     * Vertex Buffer Object (VBO) and Element Buffer Object (EBO) handles.
     * The raw MeshData is uploaded to the GPU, preparing it for subsequent
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
         * @brief Initializes all buffer objects required by OpenGL from the provided MeshData.
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
         * @brief Constructs a new OpenGLMesh instance from raw MeshData.
         *
         * @param meshData A shared_ptr to the immutable raw MeshData.
         */
        explicit OpenGLMesh(std::shared_ptr<const helios::rendering::model::MeshData> meshData);

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