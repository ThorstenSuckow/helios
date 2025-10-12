module;


export module helios.scene.Transform;

import helios.math.types;
import helios.math.transform;


export namespace helios::scene {

    /**
     * Represents a 3D transformation consisting of translation, rotation
     * and scale.
     * This class encapsulates the components of a local transform for SceneNodes.
     * It uses lazy evaluation to compute the final transform.
     * The current scale, translation and rotation is queryable.
     * Changing the transformation is always considered against an
     * "origin" of an object, that ist, a 4x4 identity matrix
     *
     * @todo use quaternions for rotation
     */
    class Transform {

        private:
        /**
         * Flag indicating whether this Transform needs to recompute
         * its transformation matrix.
         */
        bool needsUpdate_ = true;

            /**
             * The rotation component of the transformation, stored as a 4x4 matrix.
             */
        helios::math::mat4f rotation_ = helios::math::mat4f::identity();

        /**
         * The scaling component of the transformation, stored as a 3D vector.
         */
        helios::math::vec3f scale_ = helios::math::vec3f(1);

        /**
         * The translation component of the transformation, stored as a 3D vector.
         */
        helios::math::vec3f translation_ = helios::math::vec3f(0);

        /**
         * The cached 4x4 matrix, computed from scale, rotation and translation
         * (in this order, that is, T*(R*S)
         */
        mutable helios::math::mat4f cached_ = helios::math::mat4f::identity();

        /**
         * Internal helper function to compute the full 4x4 affine transformation
         * matrix.
         *
         * @return The newly computed transformation matrix.
         */
        helios::math::mat4f updateCache() const;

        public:
            ~Transform() = default;

            /**
             * Constructs a new Transform object with the default identity transformation,
             * i.e. scale (1, 1, 1), rotation 4x4_id, translate (0, 0, 0).
             */
            Transform() = default;

            /**
             * Constructs a new Transformation matrix from the specified rotation, scale and translation.
             *
             * @param rotation The initial rotation matrix.
             * @param scale  The initial scaling vector.
             * @param translation The initial translation vector.
             */
            Transform(const helios::math::mat4f& rotation,
                helios::math::vec3f scale,
                helios::math::vec3f translation
            ) noexcept;

            /**
             * Sets the rotation component of this Transform.
             * Marks this Transform object as dirty.
             *
             * @param rotation A const ref to the new rotation matrix.
             */
            void rotate(const math::mat4f& rotation) noexcept;

            /**
             * Sets the translation component of this Transform.
             * Marks this Transform object as dirty.
             *
             * @param translation A const ref to the new translation vector.
             */
            void translate(const math::vec3f& translation) noexcept;

            /**
             * Sets the scale component of this vector.
             * Marks this Transformation object as dirty.
             *
             * @param scale A const ref to the new scale vector.
             */
            void scale(const math::vec3f& scale) noexcept;

            /**
             * Returns the current rotation component of this Transform object.
             *
             * @return The current 4x4 rotation matrix.
             */
            [[nodiscard]] helios::math::mat4f rotation() const noexcept;

            /**
             * Returns the current translation component of this Transform object.
             *
             * @return The current 3D translation vector.
             */
            [[nodiscard]] helios::math::vec3f translation() const noexcept;

            /**
             * Returns the current scale component of this Transform object.
             *
             * @return The current 3D scale vector.
             */
            [[nodiscard]] helios::math::vec3f scaling() const noexcept;

            /**
             * Returns the 4x4 transformation matrix combined from scale, rotation, and translation.
             *
             * @return The current 4x4 affine transformation matrix.
             */
            [[nodiscard]] helios::math::mat4f transform() const noexcept;

    };

};