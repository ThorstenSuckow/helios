module;

#include <memory>
#include <stdexcept>


export module helios.rendering.model:Mesh;

import helios.util.Log;
import helios.math.types;
import :MeshData;

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
         * Initializes the Mesh by setting up vertex attributes and buffers.
         * This method should be called _once_ by the derived class before using
         * this Mesh in a rendering pass.
         *
         * @throws std::runtime_error if the Mesh could not be initialized.
         */
        virtual void init() = 0;

    public:

        /**
         * Creates a new Mesh instance.
         *
         * @param meshData A shared pointer to the immutable shared raw MeshData.
         *
         * @throws if MeshData is a null shared pointer, or if initializing failed.
         */
        explicit Mesh(std::shared_ptr<const MeshData> meshData)
            : meshData_(std::move(meshData)) {
            if (!meshData_) {
                helios::util::Log::error("Mesh constructor received a null MeshData shared pointer.");
                throw std::invalid_argument("Mesh constructor received a null MeshData shared pointer.");
            }
        }

        virtual ~Mesh() = default;

        /**
         * Returns a const reference to the underlying shared  MeshData.
         * The returned data is guaranteed to be a valid reference to existing data.
         * @return MeshData
         */
        [[nodiscard]] const MeshData& meshData() const noexcept {
            return *meshData_;
        }

    };

}