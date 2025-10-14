module;

#include <glad/gl.h>
#include <memory>

module helios.ext.opengl.rendering.OpenGLRenderable;

import helios.rendering.model;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.math.types;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;


using namespace helios::rendering::model;
using namespace helios::ext::opengl::rendering::model;

namespace helios::ext::opengl::rendering {

        OpenGLRenderable::OpenGLRenderable(
            std::unique_ptr<Mesh> mesh,
            std::unique_ptr<Material> material) :
            Renderable(std::move(mesh), std::move(material)) {
        }

}