module;

export module helios.rendering.model.MaterialData;

import helios.rendering.shader.Shader;
import helios.util.log.LogManager;
import helios.util.log.Logger;


#define HELIOS_LOG_SCOPE "helios::rendering::model::MaterialData"
export namespace helios::rendering::model {

    /**
     * Immutable MaterialData.
     *
     * MaterialData can safely be shared among Material instances.
     */
    class MaterialData {


    public:
        virtual ~MaterialData() = default;

        MaterialData() = default;

    };

}