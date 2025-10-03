module;

#include <memory>

module helios.ext.opengl.rendering.model.OpenGLMaterial;

import helios.rendering.model;

using namespace helios::rendering::model;

namespace helios::ext::opengl::rendering::model {

    OpenGLMaterial::OpenGLMaterial(std::shared_ptr<const MaterialData> materialData)
        : Material(std::move(materialData)) {}

}