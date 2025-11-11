/**
 * @file OpenGLRenderable.ixx
 * @brief OpenGL implementation of a Renderable wrapper.
 */
module;


#include <memory>
#include <optional>

export module helios.ext.opengl.rendering.OpenGLRenderable;

import helios.rendering.Renderable;
import helios.rendering.model.MaterialInstance;
import helios.rendering.model.Mesh;
import helios.math.types;

import helios.rendering.shader.UniformValueMap;

import helios.ext.opengl.rendering.model.OpenGLMesh;

export namespace helios::ext::opengl::rendering {

    /**
     * @brief An OpenGL specific implementation of a Renderable.
     */
    class OpenGLRenderable final : public helios::rendering::Renderable {

    public:
        /**
         * @copydoc helios::rendering::Renderable()
         */
        explicit OpenGLRenderable(
            std::shared_ptr<helios::rendering::model::Mesh> mesh,
            std::shared_ptr<helios::rendering::model::MaterialInstance> materialInstance
        );


    };
}