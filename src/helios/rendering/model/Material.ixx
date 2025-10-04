module;

#include <memory>
#include <stdexcept>

export module helios.rendering.model.Material;

import helios.rendering.model.MaterialData;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::rendering::model::Material"
export namespace helios::rendering::model {

    /**
     * Representative of a parametrizable Material instance.
     * A Material instance contains a reference to its raw shared MaterialData
     * and allows for individual configurations.
     *
     * @see [RTR, pp. 125]
     */
    class Material {


    protected:
        Material() = default;

        /**
         * Shared pointer for the raw, immutable MaterialData.
         */
        std::shared_ptr<const MaterialData> materialData_;

        /**
         * The logger used with this Material instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        virtual ~Material() = default;

        /**
         * Creates a new Material instance.
         *
         * @param materialData A shared pointer to the immutable shared MaterialData
         *
         * @throws std::invalid_argument if materialData is a null shared pointer.
         */
        explicit Material(std::shared_ptr<const MaterialData> materialData);


        /**
         * Returns a const reference to the underlying shared MaterialData.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return MaterialData
         */
        [[nodiscard]] const MaterialData& materialData() const noexcept;


        /**
         * Activates this Material instance for subsequent rendering operations.
         *
         * @see MaterialData::use()
         */
        virtual void use() const noexcept;

    };

}