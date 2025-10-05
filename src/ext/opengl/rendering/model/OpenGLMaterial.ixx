module;

#include <memory>


export module helios.ext.opengl.rendering.model.OpenGLMaterial;

import helios.rendering.model.MaterialData;
import helios.rendering.model.Material;

export namespace helios::ext::opengl::rendering::model {

    /**
     * An OpenGL specific implementation od the Material class.
     *
     * This class uses data from a shared MaterialData instance to
     * perform OpenGL specific operations
     */
    class OpenGLMaterial : public helios::rendering::model::Material{

    public:
        /**
         * Constructs a new OpenGLMaterial from shared MaterialData.
         *
         * @param materialData A shared_ptr to the immutable MaterialData.
         */
        explicit OpenGLMaterial(std::shared_ptr<const helios::rendering::model::MaterialData> materialData);


        /**
         * Activates this material and sets properties for subsequent rendering/draw calls.
         * This method delegates to the underlying MaterialData's use() function.
         */
        void use() const noexcept override;
    };

}