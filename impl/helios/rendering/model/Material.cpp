module;

#include <memory>
#include <stdexcept>

module helios.rendering.model;

import :MaterialData;
import helios.util.log;

namespace helios::rendering::model {

    Material::Material(std::shared_ptr<const MaterialData> materialData) :
    materialData_(std::move(materialData)) {
        if (!materialData_) {
            const std::string msg = "Material constructor received a null MaterialData shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    [[nodiscard]] const MaterialData& Material::materialData() const noexcept {
        return *materialData_;
    }


    void Material::use() const noexcept {
        materialData_->use();
    }

}