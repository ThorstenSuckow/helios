module;

#include <memory>
#include <stdexcept>


export module helios.rendering.model:Mesh;

import helios.util.log;
import helios.math.types;
import :MeshData;

#define HELIOS_LOG_SCOPE "helios::rendering::model::Mesh"
export namespace helios::rendering::model {

    /**
     * Representative of a renderable 3D mesh.
     * A Mesh instance contains a reference to its raw shared MeshData and allows
     * for individual configurations.
     * Concrete implementations will handle resource management.
     */
    class Mesh {

    protected:
        Mesh() = default;

        /**
         * Shared pointer to the raw, immutable MeshData.
         */
        const std::shared_ptr<const MeshData> meshData_;

        /**
         * Initializes the Mesh by setting up vertex attributes and buffers with the
         * underlying GL API.
         * This method should be called _once_ by the derived class before using
         * this Mesh in a rendering pass.
         *
         * @throws std::runtime_error if the Mesh could not be initialized.
         */
        virtual void init() = 0;

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

        virtual ~Mesh() = default;

        /**
         * Creates a new Mesh instance.
         *
         * @param meshData A shared pointer to the immutable shared raw MeshData.
         *
         * @throws std::invalid_argument if meshData is a null shared pointer.
         */
        explicit Mesh(std::shared_ptr<const MeshData> meshData);

        /**
         * Returns a const reference to the underlying shared MeshData.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return MeshData
         */
        [[nodiscard]] const MeshData& meshData() const noexcept;

    };

}