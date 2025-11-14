/**
 * @file RenderPrototype.ixx
 * @brief An immutable, shared prototype of a renderable object.
 */
module;

#include <memory>

export module helios.rendering.RenderPrototype;

import helios.rendering.model.Material;
import helios.rendering.model.Mesh;



export namespace helios::rendering {


    /**
     * @brief An immutable, shared prototype of a renderable object.
     *
     * A RenderPrototype bundles a specific Mesh (geometry) with a specific Material (shader and
     * default properties). It acts as a shared asset definition, serving as a key for efficient
     * batching and instancing in the rendering pipeline.
     *
     * Instances of RenderPrototype are designed to be shared across multiple Renderable
     * objects. They are immutable once created, ensuring consistent behavior for all
     * references and providing a stable base for per-instance overrides.
     */
    class RenderPrototype final {

    private:
        /**
         * @brief A shared pointer to the base Material definition for this prototype.
         * The Material contains shader information, default uniform values, and render states.
         * It is shared and immutable.
         */
        std::shared_ptr <const helios::rendering::model::Material> material_;

        /**
         * @brief A shared pointer to the Mesh (geometry) used by this prototype.
         * The Mesh defines the geometric shape of the object. It is shared and immutable.
         */
        std::shared_ptr<const helios::rendering::model::Mesh> mesh_;


    public:

        ~RenderPrototype() = default;

        /**
         * @brief Creates a new RenderPrototype using the specified Material and Mesh.
         *
         * @param material A shared pointer to the immutable Material this RenderPrototype uses.
         * @param mesh A shared pointer to the immutable Mesh this RenderPrototype uses.
         *
         * @throws std::invalid_argument If either material or mesh represent a nullptr.
         */
        explicit RenderPrototype(
            std::shared_ptr<const helios::rendering::model::Material> material,
            std::shared_ptr<const helios::rendering::model::Mesh> mesh
        );

        /**
         * @brief Returns const ref to the immutable Mesh used by this RenderPrototype.
         *
         * Represents a const reference to an unmodifiable Mesh object,
         * containing geometric data such as vertices and indices used
         * for rendering operations or computational tasks.
         */
        [[nodiscard]] const helios::rendering::model::Mesh& mesh() const noexcept;


        /**
         * @brief Returns a const reference to the immutable Material used by this RenderPrototype.
         * The Material contains shader information and default properties. It is guaranteed to exist
         * and be valid
         *
         * @return A const ref to the Material object.
         */
        [[nodiscard]] const helios::rendering::model::Material& material() const noexcept;

    };


}