module;

#include <memory>
#include <stdexcept>


export module helios.rendering.model.Mesh;

import helios.math.types;
import helios.rendering.model.MeshData;
import helios.util.log.LogManager;
import helios.util.log.Logger;


#define HELIOS_LOG_SCOPE "helios::rendering::model::Mesh"
export namespace helios::rendering::model {

    /**
     * @brief Representative of a renderable 3D mesh.
     * A Mesh instance contains a reference to its raw shared MeshData and allows
     * for individual configurations.
     * Concrete implementations will handle resource management.
     *
     * @see [Gre19, 631]
     */
    class Mesh {

    protected:
        Mesh() = default;

        /**
         * @brief Shared pointer to the raw, immutable MeshData.
         */
        const std::shared_ptr<const MeshData> meshData_;

        /**
         * @brief Initializes the Mesh by setting up vertex attributes and buffers with the
         * underlying GL API.
         * This method should be called _once_ by the derived class before using
         * this Mesh in a rendering pass.
         *
         * @throws std::runtime_error if the Mesh could not be initialized.
         */
        virtual void init() = 0;

        /**
         * @brief The logger used with this Mesh instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:

        virtual ~Mesh() = default;

        /**
         * @brief Creates a new Mesh instance.
         *
         * @param meshData A shared pointer to the immutable shared raw MeshData.
         *
         * @throws std::invalid_argument if meshData is a null shared pointer.
         */
        explicit Mesh(std::shared_ptr<const MeshData> meshData);

        /**
         * @brief Returns a const reference to the underlying shared MeshData.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return MeshData
         */
        [[nodiscard]] const MeshData& meshData() const noexcept;


        /**
         * @brief Returns the index count of the underlying MeshData.
         *
         * @return The number of indices handled by the raw MeshData.
         */
        [[nodiscard]] const int indexCount() const noexcept;

    };

}