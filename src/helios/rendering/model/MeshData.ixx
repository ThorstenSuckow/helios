module;

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

export module helios.rendering.model:MeshData;

import helios.util.log;
import helios.rendering.core.Vertex;
import helios.rendering.asset.shape.Shape;

using namespace helios::rendering::asset::shape;
using namespace helios::rendering::core;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MeshData"
export namespace helios::rendering::model {

    /**
     * Representative of vertex data and indices provided by geometric shapes.
     * MeshData contains references to immutable, shared Vertex Data and indices.
     */
    class MeshData {
    private:
        /**
         * Shared pointer to the raw, immutable vertices.
         */
        std::shared_ptr<const std::vector<Vertex>> vertices_ = nullptr;

        /**
         * Shared pointer to the raw, immutable indices.
         */
        std::shared_ptr<const std::vector<unsigned int>> indices_ = nullptr;

        /**
         * @todo
         * std::shared_ptr<const std::vector<Texture>> textures_;
         */

        /**
         * The logger used with this Material instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::logger(
            HELIOS_LOG_SCOPE
        );

    protected:
        MeshData() = default;

    public:
        ~MeshData() = default;

        /**
         * Creates a new MeshData instance.
         *
         * @param vertices A shared pointer to a vector of const Vertex
         * @param indices A shared pointer to a vector of indices
         *
         * @throws std::invalid_argument if either "vertices" or "indices" is a null shared pointer
         */
        explicit MeshData(
            std::shared_ptr<const std::vector<Vertex>> vertices,
            std::shared_ptr<const std::vector<unsigned int>> indices
        );


        /**
         * Creates a new MeshData instance from the specified Shape.
         *
         * @param shape A const reference to the Shape.
         *
         * @throws std::invalid_argument if either "vertices" or "indices" is a null shared pointer
         */
        explicit MeshData(const Shape& shape);


        /**
         * Returns a const reference to the underlying vertices.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return std::vector<Vertex>
         */
        [[nodiscard]] const std::vector<Vertex>& vertices() const noexcept;

        /**
         * Returns a const reference to the underlying indices.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return std::vector<unsigned int>
         */
        [[nodiscard]] const std::vector<unsigned int>& indices() const noexcept;

    };

}
