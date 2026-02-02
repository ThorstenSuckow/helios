/**
 * @file RenderPrototype.ixx
 * @brief An immutable, shared prototype of a renderable object.
 */
module;

#include <memory>
#include <stdexcept>
#include <string>

export module helios.rendering.RenderPrototype;

import helios.rendering.material.Material;
import helios.rendering.mesh.Mesh;



export namespace helios::rendering {


    /**
     * @brief An immutable, shared prototype of a renderable object.
     *
     * A `RenderPrototype` bundles a specific `Mesh` (geometry) with a specific `Material` (shader and
     * default properties). It acts as a shared asset definition, serving as a key for efficient
     * batching and instancing in the rendering pipeline.
     *
     * ## Design
     *
     * - **Immutable:** Instances are immutable once created, ensuring consistent behavior for all references.
     * - **Shared:** Designed to be shared across multiple `Renderable` objects via `std::shared_ptr`.
     * - **Batching Key:** Provides a stable base for per-instance overrides and efficient draw call batching.
     *
     * ## Ownership Model
     *
     * Both `material_` and `mesh_` use `std::shared_ptr<const T>` to enable safe sharing across
     * multiple prototypes and renderables. The const qualifier ensures immutability of the
     * referenced assets.
     *
     * ## Usage
     *
     * ```cpp
     * auto material = std::make_shared<Material>(shader, properties);
     * auto mesh = std::make_shared<OpenGLMesh>(vertices, indices);
     *
     * auto prototype = std::make_shared<RenderPrototype>(material, mesh);
     *
     * // Share across multiple renderables
     * MeshRenderable obj1(prototype);
     * MeshRenderable obj2(prototype, colorOverride);
     * ```
     *
     * @see MeshRenderable
     * @see Material
     * @see Mesh
     */
    class RenderPrototype final {

    private:
        /**
         * @brief A shared pointer to the base Material definition for this prototype.
         * The Material contains shader information, default uniform values, and render states.
         * It is shared and immutable.
         */
        std::shared_ptr <const helios::rendering::material::Material> material_;

        /**
         * @brief A shared pointer to the Mesh (geometry) used by this prototype.
         * The Mesh defines the geometric shape of the object. It is shared and immutable.
         */
        std::shared_ptr<const helios::rendering::mesh::Mesh> mesh_;


    public:

        ~RenderPrototype() = default;

        /**
         * @brief Creates a new RenderPrototype using the specified Material and Mesh.
         *
         * Both parameters are moved into the prototype to avoid unnecessary reference
         * count increments. The prototype takes shared ownership of both assets.
         *
         * @param material A shared pointer to the immutable Material this RenderPrototype uses.
         * @param mesh A shared pointer to the immutable Mesh this RenderPrototype uses.
         *
         * @throws std::invalid_argument If either material or mesh represent a nullptr.
         */
        explicit RenderPrototype(
            std::shared_ptr<const helios::rendering::material::Material> material,
            std::shared_ptr<const helios::rendering::mesh::Mesh> mesh
        ) :
        material_(std::move(material)),
        mesh_(std::move(mesh)) {

            if (!material_) {
                throw std::invalid_argument("RenderPrototype received material nullptr");
            }
            if (!mesh_) {
                throw std::invalid_argument("RenderPrototype received mesh nullptr");
            }

        }

        /**
         * @brief Returns a const reference to the immutable Mesh used by this RenderPrototype.
         *
         * The Mesh contains geometric data such as vertices and indices used for rendering.
         * This method returns a reference to avoid shared_ptr copy overhead.
         *
         * @return A const reference to the Mesh object.
         */
        [[nodiscard]] const helios::rendering::mesh::Mesh& mesh() const noexcept {
            return *mesh_;
        }


        /**
         * @brief Returns a const reference to the immutable Material used by this RenderPrototype.
         *
         * The Material contains shader information and default properties. It is guaranteed
         * to exist and be valid. This method returns a reference to avoid shared_ptr copy overhead.
         *
         * @return A const reference to the Material object.
         */
        [[nodiscard]] const helios::rendering::material::Material& material() const noexcept {
            return *material_;
        }

    };


}