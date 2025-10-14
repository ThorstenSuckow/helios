module;


#include <memory>
#include <optional>

export module helios.ext.opengl.rendering.OpenGLRenderable;

import helios.rendering.Renderable;
import helios.rendering.model.Material;
import helios.rendering.model.Mesh;
import helios.math.types;

import helios.rendering.shader.UniformValueMap;

import helios.ext.opengl.rendering.model.OpenGLMesh;

export namespace helios::ext::opengl::rendering {

    /**
     * An OpenGL specific implementation of a Renderable.
     */
    class OpenGLRenderable : public helios::rendering::Renderable {

    public:
        /**
         * @copydoc helios::rendering::Renderable()
         */
        explicit OpenGLRenderable(std::unique_ptr<helios::rendering::model::Mesh> mesh,
            std::unique_ptr<helios::rendering::model::Material> material);


    };
}