module;

#include <memory>


export module helios.ext.opengl.rendering.model.OpenGLMaterial;

import helios.rendering.model.MaterialData;
import helios.rendering.model.Material;

export namespace helios::ext::opengl::rendering::model {

    /**
     * OpenGL Material representative.
     */
    class OpenGLMaterial : public helios::rendering::model::Material{

    public:

        explicit OpenGLMaterial(std::shared_ptr<const helios::rendering::model::MaterialData> materialData);

    };

}