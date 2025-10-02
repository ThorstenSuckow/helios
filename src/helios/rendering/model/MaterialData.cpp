module;

#include <memory>
#include <string>
#include <stdexcept>

module helios.rendering.model;

import :Shader;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MaterialData"
namespace helios::rendering::model {

    /**
     * Creates a new MaterialData instance.
     *
     * @param shader A shared pointer to the immutable Shader used by
     * this instance.
     *
     * @throws std::invalid_argument if shader is a null shared pointer.
     */
    MaterialData::MaterialData(std::shared_ptr<const Shader> shader) :
    shader_(std::move(shader)) {
        if (!shader_) {
            const std::string msg = "MaterialData constructor received a null Shader shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }

    }


    /**
     * Returns a const reference to the underlying shared Shader.
     *
     * The returned data is guaranteed to be a valid reference to existing data.
     *
     * @return Shader
     */
    [[nodiscard]] const Shader& MaterialData::shader() const noexcept {
        return *shader_;
    }


    /**
     * Activates this material for subsequent rendering operations.
     *
     * @see Shader::use()
     */
    void MaterialData::use() const noexcept {
        shader_->use();
    }

}