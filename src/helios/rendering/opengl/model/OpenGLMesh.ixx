module;

#include <memory>

export module helios.rendering.opengl.model:OpenGLMesh;

import helios.rendering.model;
import helios.rendering.core.Vertex;

using namespace helios::rendering::model;
using namespace helios::rendering::core;

export namespace helios::rendering::opengl::model {

    /**
     * Representative of an OpenGLMesh.
     *
     */
    class OpenGLMesh : public Mesh{

    protected:
        /**
         * Vertex Array Object handle.
         */
        const unsigned int vao_;

        /**
         * Vertex Buffer Object handle.
         */
        const unsigned int vbo_;

        /**
         * Element Buffer Object handle.
         */
        const unsigned int ebo_;

        /**
         * Helper function for generating a Vertex Array Object identifier.
         *
         * @return Vertex Array Object handle.
         */
        static unsigned int generateGLVertexArray() noexcept;


        /**
         * Helper function for generating a Buffer Object identifier,
         * to be used with vertex buffer or element buffer.
         *
         * @return Vertex Array Object handle.
         */
        static unsigned int generateGLBuffer() noexcept;


        void init() override;

    public:

        explicit OpenGLMesh(std::shared_ptr<const MeshData> meshData);

        /**
         * Frees allocated resources bv this instance.
         */
        ~OpenGLMesh() override;

        /**
         * Returns the OpenGL Vertex Array Object Handle.
         *
         * @return VAO handle
         */
        [[nodiscard]] const unsigned int& vao() const noexcept;

        /**
         * Returns the OpenGL Vertex Buffer Object handle.
         *
         * @return VBO handle
         */
        [[nodiscard]] const unsigned int& vbo() const noexcept;

        /**
         * Returns the OpenGL Element Buffer Object handle for indexed rendering.
         *
         * @return EBO handle
         */
        [[nodiscard]] const unsigned int& ebo() const noexcept;

    };

}