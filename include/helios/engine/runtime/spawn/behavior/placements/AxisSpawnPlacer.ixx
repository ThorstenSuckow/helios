/**
 * @file AxisSpawnPlacer.ixx
 * @brief Spawn placer that arranges entities along an axis.
 */
module;

#include <cassert>
#include <algorithm>

export module helios.engine.runtime.spawn.behavior.placements.AxisSpawnPlacer;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.behavior.SpawnPlacer;
import helios.util.Random;
import helios.math;
import helios.engine.ecs.EntityHandle;

import helios.engine.runtime.world.UpdateContext;


export namespace helios::engine::runtime::spawn::behavior::placements {

    /**
     * @brief Spawn placer that distributes entities evenly along an axis.
     *
     * @details AxisSpawnPlacer positions spawned entities along a normalized
     * direction vector starting from an origin point. Entities are distributed
     * evenly between the origin and the environment boundary intersection.
     *
     * ## Positioning Algorithm
     *
     * 1. Calculates where the axis intersects the environment bounds
     * 2. Computes available length accounting for entity size
     * 3. Distributes entities evenly along this length based on cursor position
     *
     * ## Use Cases
     *
     * - **Formation spawning:** Line formations of enemies
     * - **Wave patterns:** Entities spawning along a direction
     * - **Corridor spawning:** Entities placed along a path
     *
     * ## Usage
     *
     * ```cpp
     * // Spawn entities along positive X-axis from origin
     * auto placer = std::make_unique<AxisSpawnPlacer>(
     *     helios::math::vec3f{1.0f, 0.0f, 0.0f},  // axis (normalized)
     *     helios::math::vec3f{-100.0f, 0.0f, 0.0f} // origin
     * );
     *
     * // With 5 entities: evenly spaced from origin to right boundary
     * ```
     *
     * @note The axis vector must be normalized.
     * @note The origin must be within the environment bounds.
     *
     * @see SpawnPlacer
     * @see DistributedSpawnPlacer
     */
    class AxisSpawnPlacer final : public SpawnPlacer {

        /**
         * @brief Normalized direction vector defining the spawn axis.
         */
        helios::math::vec3f axis_;

        /**
         * @brief Starting point for entity placement.
         */
        helios::math::vec3f origin_{};

    public:

        /**
         * @brief Constructs an AxisSpawnPlacer with axis and origin.
         *
         * @param axis Normalized direction vector for entity distribution.
         * @param origin Starting point for the axis within environment bounds.
         *
         * @pre `axis` must be normalized.
         */
        explicit AxisSpawnPlacer(const helios::math::vec3f axis, const helios::math::vec3f origin)
        : axis_(axis), origin_(origin) {
            assert(axis.isNormalized() && "Axis must be normalized.");
        }

        /**
         * @brief Calculates spawn position along the axis.
         *
         * @details Distributes entities evenly along the axis from origin to
         * the environment boundary. Uses the cursor position to determine
         * placement within the available range.
         *
         * @param guid Unique identifier of the entity (unused).
         * @param gameObjectBounds Bounding box of the entity being spawned.
         * @param environmentBounds Bounding box of the spawn environment.
         * @param cursor Current spawn batch cursor with position and count.
         * @param spawnContext Context for the spawn operation (unused).
         *
         * @return World position for the spawned entity.
         *
         * @pre Origin must be within environment bounds.
         */
        helios::math::vec3f getPosition(
            const helios::engine::ecs::EntityHandle& entityHandle,
            const helios::math::aabbf& gameObjectBounds,
            const helios::math::aabbf& environmentBounds,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            assert(environmentBounds.contains(origin_) && "origin is not within bounding box");

            const float top    = environmentBounds.max()[1];
            const float bottom = environmentBounds.min()[1];
            const float left   = environmentBounds.min()[0];
            const float right  = environmentBounds.max()[0];
            const float near   = environmentBounds.min()[2];
            const float far    = environmentBounds.max()[2];

            const float height = std::abs(top - bottom);
            const float width = std::abs(right - left);

            float t_x = std::numeric_limits<float>::infinity();
            if (axis_[0] < 0) {
                t_x = ((left - origin_[0]) / axis_[0]);
            } else if (axis_[0] > 0 ){
                t_x = ((right - origin_[0]) / axis_[0]);
            }

            float t_y = std::numeric_limits<float>::infinity();
            if (axis_[1] < 0) {
                t_y = ((bottom - origin_[1]) / axis_[1]);
            } else if (axis_[1] > 0 ){
                t_y = ((top - origin_[1]) / axis_[1]);
            }

            float t_z =std::numeric_limits<float>::infinity();
            if (axis_[2] < 0) {
                t_z = ((near - origin_[2]) / axis_[2]);
            } else if (axis_[2] > 0 ){
                t_z = ((far - origin_[2]) / axis_[2]);
            }

            const auto t = std::min(std::min(t_x, t_y), t_z);

            // compute the projection of the aaabb onto the direction vector.
            // we use absolute values of the direction vector to make sure
            // we effectively measure dimensions, not projections relative
            // to the origin direction
            const auto objectHalfSize = gameObjectBounds.size() * 0.5f;
            const auto projection = objectHalfSize.dot(helios::math::vec3f{
                std::abs(axis_[0]), std::abs(axis_[1]), std::abs(axis_[2])
            });

            float availableLength =  t - projection;
            availableLength =  availableLength < 0 ? 0 : availableLength;

            const auto pos = axis_ * availableLength *
                (static_cast<float>(cursor.position)/static_cast<float>(
                    cursor.spawnCount > 0 ? cursor.spawnCount - 1 :1)
                );


            const auto center = gameObjectBounds.translate(
                    helios::math::vec3f{origin_[0] + pos[0], origin_[1] + pos[1], origin_[2] + pos[2]}
                ).center();

            return center;
        }


    };

}