module;

#include <memory>
#include <stdexcept>

module helios.rendering.model.Material;


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

}