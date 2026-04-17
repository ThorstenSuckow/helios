/**
 * @file MeshRenderer.ixx
 * @brief Abstract rendering interface for mesh draw commands.
 */
module;

export module helios.rendering.mesh.MeshRenderer;

import helios.rendering.mesh.commands.MeshRenderCommand;

import helios.rendering.shader.UniformValueMap;

using namespace helios::rendering::shader;
using namespace helios::rendering::mesh::commands;
export namespace helios::rendering::mesh {


    /**
     * @brief Abstract base class for mesh rendering operations.
     *
     * MeshRenderer defines the interface for rendering mesh geometry using a specific
     * graphics API. Implementations of this class are responsible for translating
     * abstract render commands into API-specific draw calls.
     *
     * This class follows the Strategy pattern, allowing the rendering backend to be
     * swapped without affecting higher-level rendering logic.
     *
     * Example usage:
     * ```cpp
     * // Typically used polymorphically through a concrete implementation
     * std::unique_ptr<MeshRenderer> renderer = std::make_unique<OpenGLMeshRenderer>();
     * renderer->render(command, frameUniforms);
     * ```
     *
     * @see helios::ext::opengl::rendering::OpenGLMeshRenderer
     */
    class MeshRenderer {
    public:

        /**
         * @brief Virtual destructor for proper cleanup in derived classes.
         */
        virtual ~MeshRenderer() = default;


        /**
         * @brief Renders a mesh using the provided render command and frame-level uniforms.
         *
         * Implementations should apply the shader, bind the mesh geometry, set uniform values,
         * and issue the appropriate draw call for the target graphics API.
         *
         * @param command The render command containing mesh, material, and per-object uniform data.
         * @param frameUniformValues Frame-level uniform values (e.g., view/projection matrices).
         */
        virtual void render(
            const MeshRenderCommand& command,
            const UniformValueMap& frameUniformValues
        ) noexcept = 0;
    };
}