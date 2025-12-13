/**
 * @file InheritTransform.ixx
 * @brief Defines transform inheritance flags for scene graph hierarchies.
 */
module;

#include <cstdint>

export module helios.scene.InheritTransform;


export namespace helios::scene::InheritTransform {

    /**
     * @brief Bitmask enum controlling which transform components a child node inherits from its parent.
     *
     * When a SceneNode is attached to a parent, its final world transform is computed by
     * combining the parent's world transform with the child's local transform. The `Inherit`
     * flags determine which components (Translation, Rotation, Scale) of the parent transform
     * are applied to the child.
     *
     * This enables flexible scene graph behaviors such as:
     * - A camera that follows an object's position but maintains its own orientation (`Inherit::Translation`)
     * - UI elements that inherit full transforms (`Inherit::All`)
     * - Objects that only inherit rotation for billboard effects (`Inherit::Rotation`)
     *
     * Example usage:
     * ```cpp
     * using namespace helios::scene::InheritTransform;
     *
     * // Camera follows spaceship position only
     * cameraNode->setInheritance(Inherit::Translation);
     *
     * // Full transform inheritance (default)
     * childNode->setInheritance(Inherit::All);
     *
     * // Combine specific flags
     * node->setInheritance(Inherit::Translation | Inherit::Rotation);
     * ```
     */
    enum class Inherit : uint8_t {
        /**
         * @brief Inherit only the translation component from the parent.
         *
         * The child's world position is offset by the parent's world position,
         * but rotation and scale remain unaffected by the parent.
         */
        Translation = 1 << 0,

        /**
         * @brief Inherit only the rotation component from the parent.
         *
         * The child's orientation is combined with the parent's orientation,
         * but position and scale remain unaffected.
         */
        Rotation = 1 << 1,

        /**
         * @brief Inherit only the scale component from the parent.
         *
         * The child's scale is multiplied by the parent's scale,
         * but position and rotation remain unaffected.
         */
        Scale = 1 << 2,

        /**
         * @brief Inherit all transform components (Translation, Rotation, Scale).
         *
         * This is the default behavior where the child's world transform is
         * the full composition of parent and local transforms.
         */
        All = Translation | Rotation | Scale
    };

    /**
     * @brief Combines two Inherit flags using bitwise OR.
     *
     * @param a The first inheritance flag.
     * @param b The second inheritance flag.
     *
     * @return The combined inheritance mask.
     */
    inline Inherit operator|(Inherit a, Inherit b) {
        return static_cast<Inherit>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }


    /**
     * @brief Checks if a specific inheritance flag is set in a mask.
     *
     * @param mask The inheritance mask to check against.
     * @param flag The specific flag to test for.
     *
     * @return `true` if the flag is present in the mask, `false` otherwise.
     *
     * Example usage:
     * ```cpp
     * using namespace helios::scene::InheritTransform;
     *
     * Inherit mode = Inherit::Translation | Inherit::Rotation;
     *
     * if (has(mode, Inherit::Translation)) {
     *     // Apply parent translation...
     * }
     * ```
     */
    inline bool has(Inherit mask, Inherit flag) {
        return (static_cast<uint8_t>(mask) & static_cast<uint8_t>(flag)) != 0;
    }
}
