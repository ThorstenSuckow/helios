module;

#include <memory>


export module helios.rendering.opengl.model:OpenGLMaterial;

import helios.rendering.model;


using namespace helios::rendering::model;

export namespace helios::rendering::opengl::model {

    /**
     * OpenGL Material representative.
     */
    class OpenGLMaterial : public Material{

    public:

        explicit OpenGLMaterial(std::shared_ptr<const MaterialData> materialData);

    };

}