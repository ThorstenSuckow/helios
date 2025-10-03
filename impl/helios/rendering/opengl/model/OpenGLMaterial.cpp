module;

#include <memory>

module helios.rendering.opengl.model;

import helios.rendering.model;


using namespace helios::rendering::model;

namespace helios::rendering::opengl::model {

    OpenGLMaterial::OpenGLMaterial(std::shared_ptr<const MaterialData> materialData)
        : Material(std::move(materialData)) {}

}