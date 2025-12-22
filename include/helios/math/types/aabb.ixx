/**
 * @file aabb.ixx
 * @brief Axis-Aligned Bounding Box (AABB) type and transformations.
 */
module;

#include <memory>

export module helios.math.types:aabb;

import :vec3;
import :mat4;
import helios.math.concepts;
import helios.math.traits.FloatingPointType;

export namespace helios::math {

    /**
     * @brief Axis-Aligned Bounding Box for spatial culling and collision detection.
     *
     * An AABB is defined by two corner points: a minimum and maximum point in 3D space.
     * Depending on the use case, the box is aligned to the world or local coordinate axes.
     *
     * Common use cases include frustum culling, broad-phase collision detection,
     * and spatial partitioning in scene graphs.
     *
     * @tparam T The numeric type of the vector components (typically `float` or `double`).
     *
     * @see [Gla95, pp. 548-550] Glassner, A. (1995). Graphics Gems
     * @see [DP11, pp. 304-311] Dunn, F., & Parberry, I. (2011). 3D Math Primer for Graphics and Game Development
     */
    template<helios::math::Numeric T>
    struct aabb {

    private:
        /**
         * @brief Minimum corner point of the bounding box.
         *
         * Contains the smallest x, y, and z coordinates across all points within the AABB.
         */
        helios::math::vec3<T> min_;

        /**
         * @brief Maximum corner point of the bounding box.
         *
         * Contains the largest x, y, and z coordinates across all points within the AABB.
         */
        helios::math::vec3<T> max_;

    public:

        /**
         * @brief Constructs an empty AABB with inverted min/max values.
         *
         * The AABB is initialized with min set to the maximum representable value
         * and max set to the minimum representable value. This allows the first
         * `add()` call to properly establish the initial bounds.
         */
        constexpr aabb() noexcept
        : min_{std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max()},
          max_{std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()}
        {}

        /**
         * @brief Constructs an AABB from individual component values.
         *
         * @param minX Minimum X coordinate.
         * @param minY Minimum Y coordinate.
         * @param minZ Minimum Z coordinate.
         * @param maxX Maximum X coordinate.
         * @param maxY Maximum Y coordinate.
         * @param maxZ Maximum Z coordinate.
         */
        constexpr aabb(const T minX, const T minY, const T minZ, const T maxX, const T maxY, const T maxZ) noexcept
        : min_{minX, minY, minZ},
          max_{maxX, maxY, maxZ}
        {}

        /**
         * @brief Constructs an AABB from two corner points.
         *
         * @param min The minimum corner point (smallest x, y, z).
         * @param max The maximum corner point (largest x, y, z).
         */
        constexpr aabb(const helios::math::vec3<T> min, const helios::math::vec3<T> max) noexcept
        : min_(min),
          max_(max)
        {}

        /**
         * @brief Returns the minimum corner point of the AABB.
         *
         * @return Const reference to the minimum corner point.
         */
        [[nodiscard]] constexpr const helios::math::vec3<T>& min() const noexcept {
            return min_;
        }

        /**
         * @brief Returns the maximum corner point of the AABB.
         *
         * @return Const reference to the maximum corner point.
         */
        [[nodiscard]] constexpr const helios::math::vec3<T>& max() const noexcept {
            return max_;
        }

        /**
         * @brief Computes the center point of the AABB.
         *
         * @return The center point, calculated as `(min + max) / 2`.
         */
        [[nodiscard]] constexpr helios::math::vec3<T> getCenter() const noexcept {
            return (min_ + max_) * static_cast<T>(0.5);
        }

        /**
         * @brief Computes the size of the AABB over all axes.
         *
         * @return A vector representing the width, height, and depth of the AABB.
         */
        [[nodiscard]] constexpr helios::math::vec3<T> getSize() const noexcept {
            return max_ - min_;
        }

        /**
         * @brief Checks if this AABB fully contains another AABB.
         *
         * @details Tests whether all corners of the specified bounding box lie within
         * the bounds of this AABB. Both minimum and maximum corners must be contained.
         *
         * @param box The AABB to test for containment.
         *
         * @return True if the specified box is fully contained within this AABB, false otherwise.
         */
        [[nodiscard]] constexpr bool contains(const helios::math::aabb<T>& box) const noexcept {

            auto v_min = box.min();
            auto v_max = box.max();

            return v_min[0] >= min_[0] && v_min[1] >= min_[1] && v_min[2] >= min_[2] &&
                   v_max[0] <= max_[0] && v_max[1] <= max_[1] && v_max[2] <= max_[2];

        }

        /**
         * @brief Expands the AABB to include a given point.
         *
         * Updates the minimum and maximum bounds to ensure the specified point
         * lies within the AABB. Each component is compared independently:
         * - If a component of the point is smaller than the current minimum, the minimum is updated.
         * - If a component of the point is larger than the current maximum, the maximum is updated.
         *
         * @param point The 3D point to include in the bounding box.
         */
        void add(const helios::math::vec3<T>& point) noexcept {
            min_[0] = std::min(min_[0], point[0]);
            min_[1] = std::min(min_[1], point[1]);
            min_[2] = std::min(min_[2], point[2]);

            max_[0] = std::max(max_[0], point[0]);
            max_[1] = std::max(max_[1], point[1]);
            max_[2] = std::max(max_[2], point[2]);
        }

        /**
         * @brief Transforms the AABB by a 4x4 transformation matrix.
         *
         * Applies a transformation matrix to the AABB and returns a new axis-aligned bounding box
         * that fully contains the transformed original. This method preserves the axis-aligned property
         * by computing new min/max bounds from the transformed corners.
         *
         * The algorithm efficiently transforms only the min/max corners instead of all 8 vertices,
         * using Arvo's optimization technique from Graphics Gems.
         *
         * @param mat The 4x4 transformation matrix to apply.
         *
         * @return A new AABB that fully contains the transformed bounding box.
         *
         * @see [Gla95, pp. 548-550] "Transforming Axis-Aligned Bounding Boxes" by James Arvo
         */
        [[nodiscard]] aabb<T> transform(const mat4<T>& mat) const noexcept {

            const T translationX = static_cast<T>(mat(0, 3));
            const T translationY = static_cast<T>(mat(1, 3));
            const T translationZ = static_cast<T>(mat(2, 3));

            vec3<T> newMin = {translationX, translationY, translationZ};
            vec3<T> newMax = newMin;

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {

                    T val = static_cast<T>(mat(j, i));

                    T e = val * min_[i];
                    T f = val * max_[i];

                    if (e < f) {
                        newMin[j] += e;
                        newMax[j] += f;
                    } else {
                        newMin[j] += f;
                        newMax[j] += e;
                    }
                }
            }

            return aabb<T>(newMin, newMax);
        }


    };

    /**
     * @brief Single-precision floating-point AABB.
     */
    using aabbf = aabb<float>;

    /**
     * @brief Integer AABB.
     */
    using aabbi = aabb<int>;

    /**
     * @brief Double-precision floating-point AABB.
     */
    using aabbd = aabb<double>;

} // namespace helios::math
