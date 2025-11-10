/**
 * @brief Mesh abstraction: immutable vertex/index data and configuration.
 */
module;

#include <memory>
#include <stdexcept>
#include <vector>


export module helios.rendering.model.Mesh;

import helios.math.types;
import helios.rendering.model.config.MeshConfig;
import helios.rendering.Vertex;
import helios.rendering.asset.shape.Shape;
import helios.util.log.LogManager;
import helios.util.log.Logger;


#define HELIOS_LOG_SCOPE "helios::rendering::model::Mesh"
export namespace helios::rendering::model {

    /**
     * @brief Representative of a renderable 3D mesh.
     *
     * A Mesh instance contains vertex data and indices provided by geometric shapes.
     * Meshes contain references to immutable, shared vertex data and indices.
     * Concrete implementations will handle resource management.
     *
     * @see [Gre19, 631]
     */
    class Mesh {

    protected:

        Mesh() = default;

        /**
         * @brief Shared pointer to the raw, immutable vertices.
         */
        std::shared_ptr<const std::vector<Vertex>> vertices_ = nullptr;

        /**
         * @brief Shared pointer to the raw, immutable indices.
         */
        std::shared_ptr<const std::vector<unsigned int>> indices_ = nullptr;

        /**
         * @brief Shared pointer to the MeshConfig used with this Mesh.
         */
        std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig_;

        /**
         * @todo
         * std::shared_ptr<const std::vector<Texture>> textures_;
         */

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
         * @param vertices A shared_ptr to a vector of Vertex.
         * @param indices A shared_ptr to a vector of indices.
         * @param meshConfig A shared_ptr to the MeshConfig used with this Mesh.
         *
         * @throws std::invalid_argument if either "vertices", "indices" or meshConfig is a null shared_ptr
         */
        explicit Mesh(
            std::shared_ptr<const std::vector<Vertex>> vertices,
            std::shared_ptr<const std::vector<unsigned int>> indices,
            std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
        );

        /**
         * @brief Creates a new Mesh instance from the specified Shape.
         *
         * @param shape A const reference to the Shape.
         * @param meshConfig A shared_ptr to the MeshConfig used with this Mesh.
         *
         * @throws std::invalid_argument if meshConfig is a null shared_ptr, or if the
         * shape contained null data.
         */
        explicit Mesh(
            const helios::rendering::asset::shape::Shape& shape,
            std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
        );

        /**
         * @brief Returns the index count of this Mesh.
         *
         * @return The number of indices handled by this Mesh.
         */
        [[nodiscard]] const unsigned int indexCount() const noexcept;

        /**
         * @brief Returns a const reference to the underlying vertices.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return A const ref to the vector of Vertex.
         */
        [[nodiscard]] const std::vector<Vertex>& vertices() const noexcept;

        /**
         * @brief Returns a const reference to the underlying indices.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return A const ref to the vector of indices.
         */
        [[nodiscard]] const std::vector<unsigned int>& indices() const noexcept;

        /**
         * @brief Returns a const reference to MeshConfig used with this Mesh.
         * The returned MeshConfig is guaranteed to be a valid reference to existing data.
         *
         * @return The MeshConfig used with this Mesh.
         */
        [[nodiscard]] const helios::rendering::model::config::MeshConfig& meshConfig() const noexcept;

    };

}