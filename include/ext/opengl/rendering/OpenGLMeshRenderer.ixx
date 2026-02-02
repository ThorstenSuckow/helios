/**
 * @file OpenGLMeshRenderer.ixx
 * @brief OpenGL implementation of the MeshRenderer interface for rendering mesh geometry.
 */
module;

#include <cassert>
#include <glad/gl.h>
#include <ostream>
#include <ranges>



export module helios.ext.opengl.rendering.OpenGLMeshRenderer;

import helios.rendering;
import helios.ext.opengl.rendering.OpenGLEnumMapper;
import helios.ext.opengl.rendering.shader;
import helios.ext.opengl.rendering.model;


export namespace helios::ext::opengl::rendering {


    /**
     * @brief OpenGL implementation of the MeshRenderer interface.
     *
     * `OpenGLMeshRenderer` provides the concrete rendering logic for mesh geometry using
     * the OpenGL graphics API. It translates abstract `MeshRenderCommand` objects into
     * OpenGL-specific draw calls, handling shader binding, uniform application, and
     * VAO state management.
     *
     * ## State Caching
     *
     * This class implements state caching to minimize redundant OpenGL state changes:
     * - Shader programs are only activated when they differ from the previously used shader.
     * - VAO bindings are only updated when rendering a mesh with a different VAO.
     *
     * ## Usage
     *
     * ```cpp
     * OpenGLMeshRenderer renderer;
     * MeshRenderCommand command = createRenderCommand();
     * UniformValueMap frameUniforms = {{"viewMatrix", viewMat}, {"projMatrix", projMat}};
     * renderer.render(command, frameUniforms);
     * ```
     *
     * ## Ownership
     *
     * This class is typically owned by `OpenGLDevice` and should not be instantiated
     * directly in most use cases.
     *
     * @note The renderer expects all shaders and meshes to be OpenGL-specific implementations
     *       (`OpenGLShader` and `OpenGLMesh`). Using incompatible types will trigger assertions.
     *
     * @see helios::rendering::mesh::MeshRenderer
     * @see helios::rendering::mesh::MeshRenderCommand
     * @see OpenGLDevice
     */
    class OpenGLMeshRenderer : public helios::rendering::mesh::MeshRenderer {



        friend class OpenGLDevice;

        /**
         * @brief Cached pointer to the last used shader for state optimization.
         *
         * Used to avoid redundant shader program activations. Reset at the beginning
         * of each render pass.
         */
        mutable const helios::ext::opengl::rendering::shader::OpenGLShader* lastShader_ = nullptr;

        /**
         * @brief Cached VAO ID for state optimization.
         *
         * Used to avoid redundant VAO bindings. Reset at the beginning of each render pass.
         */
        mutable unsigned int lastVao_ = 0;

        /**
         * @brief Initializes the renderer.
         *
         * Currently a no-op placeholder for future initialization logic.
         */
        void init() {

        }


        /**
         * @brief Resets cached rendering state at the beginning of a render pass.
         *
         * This ensures proper shader and VAO binding even when the render queue
         * contents change between frames. Called internally by `OpenGLDevice` before
         * processing the render queue.
         *
         * @param renderPass The render pass being started (currently unused, reserved for future use).
         */
        void beginRenderPass(helios::rendering::RenderPass& renderPass) const noexcept {
            // Reset cached state at the beginning of each render pass
            // to ensure proper shader and VAO binding even when render queue contents change

            lastShader_ = nullptr;
            lastVao_ = 0;
        }

    public:


        /**
         * @brief Destructor that unbinds any currently bound VAO.
         *
         * Ensures clean OpenGL state upon destruction.
         */
        ~OpenGLMeshRenderer() override {

            glBindVertexArray(0);
        }


        /**
         * @brief Renders a mesh using the provided render command and frame-level uniforms.
         *
         * This method performs the following steps:
         * 1. Extracts the shader and mesh from the render command's prototype.
         * 2. Activates the shader if it differs from the previously used shader (state caching).
         * 3. Applies frame-level, object-level, and material-level uniform values.
         * 4. Binds the mesh's VAO if it differs from the previously bound VAO (state caching).
         * 5. Issues a `glDrawElements` call with the appropriate primitive type.
         *
         * @param command The render command containing the render prototype with mesh,
         *                material, and per-object uniform data.
         * @param frameUniformValues Frame-level uniform values shared across all objects
         *                           (e.g., view and projection matrices).
         *
         * @note If the render command does not contain a valid render prototype, the
         *       method returns without issuing any draw calls.
         *
         * @see MeshRenderCommand
         * @see UniformValueMap
         */
        void render(
            const helios::rendering::mesh::MeshRenderCommand& command,
            const helios::rendering::shader::UniformValueMap& frameUniformValues)  noexcept {

            if (const auto renderPrototype_ptr = command.renderPrototype()) {
                const auto& baseShader = renderPrototype_ptr->material().shader();
                const auto& baseMesh = renderPrototype_ptr->mesh();

                const auto* shader = static_cast<const helios::ext::opengl::rendering::shader::OpenGLShader*>(&baseShader);
                assert(shader && "Unexpected failure when casting to OpenGLShader.");

                if (shader != lastShader_) {
                    shader->use();
                    lastShader_ = shader;
                }

                shader->applyUniformValues(frameUniformValues);
                shader->applyUniformValues(command.objectUniformValues());
                shader->applyUniformValues(command.materialUniformValues());

                const auto* mesh = static_cast<const helios::ext::opengl::rendering::model::OpenGLMesh*>(&baseMesh);
                assert(mesh && "Unexpected failure when casting to OpenGLMesh.");

                const auto [primitiveType] = mesh->meshConfig();

                if (mesh->vao() != lastVao_) {
                    glBindVertexArray(mesh->vao());
                    lastVao_ = mesh->vao();
                }

                glDrawElements(helios::ext::opengl::rendering::OpenGLEnumMapper::toOpenGL(primitiveType), mesh->indexCount(), GL_UNSIGNED_INT, nullptr);
            }

        };




    };
}