module;

#include <memory>
#include <string>
#include <stdexcept>

export module helios.rendering.model:MaterialData;

import :Shader;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MaterialData"
export namespace helios::rendering::model {

    /**
     * Immutable MaterialData representing shader information.
     *
     * MaterialData can safely be shared among Material instances.
     */
    class MaterialData {


    protected:
        MaterialData() = default;

        /**
         * Shared pointer to the immutable Shader.
         */
        std::shared_ptr<const Shader> shader_;

        /**
         * The logger used with this MaterialData instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::logger(
            HELIOS_LOG_SCOPE
        );
    public:
        virtual ~MaterialData() = default;


        /**
         * Creates a new MaterialData instance.
         *
         * @param shader A shared pointer to the immutable Shader used by
         * this instance.
         *
         * @throws std::invalid_argument if shader is a null shared pointer.
         */
        explicit MaterialData(std::shared_ptr<const Shader> shader);


        /**
         * Returns a const reference to the underlying shared Shader.
         *
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return Shader
         */
        [[nodiscard]] virtual const Shader& shader() const noexcept;


        /**
         * Activates this material for subsequent rendering operations.
         *
         * @see Shader::use()
         */
        virtual void use() const noexcept;

    };

}