/**
 * @file GridCollisionDetectionSystem.ixx
 * @brief Spatial-partitioning based collision detection using a uniform 3D grid.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <unordered_set>
#include <vector>

export module helios.engine.game.systems.physics.GridCollisionDetectionSystem;

import helios.engine.game.System;
import helios.engine.game.UpdateContext;
import helios.engine.game.GameObjectFilter;
import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;


import helios.engine.game.event.TriggerCollisionEvent;
import helios.engine.game.event.SolidCollisionEvent;

import helios.engine.game.components.physics.CollisionComponent;
import helios.engine.game.components.physics.AabbColliderComponent;

import helios.util.Guid;
import helios.math;


export namespace helios::engine::game::systems::physics {

    /**
     * @brief Collision detection system using uniform spatial partitioning for Broadphase and
     * AABB overlappings in the Narrowphase.
     *
     * This system implements a grid-based spatial partitioning approach for efficient collision
     * detection, following the principles outlined in Ericson's "Real-Time Collision Detection"
     * (Chapter 7). The algorithm divides the world into a uniform 3D grid of cells and assigns
     * each collidable entity to the cells it overlaps.
     *
     * The detection process consists of two phases:
     * 1. **Broadphase:** Entities are inserted into grid cells based on their AABB. Only entities
     *    sharing the same cell are considered potential collision pairs.
     * 2. **Narrowphase:** For each cell with multiple candidates, AABB intersection tests
     *    determine actual collisions.
     *
     * Collision events are published to the `UpdateContext`'s event sink, distinguishing between:
     * - **Solid collisions:** Symmetric collisions where both entities can physically interact.
     * - **Trigger collisions:** Asymmetric collisions for gameplay logic (e.g., pickups, zones).
     *
     * During the broadphase, the system uses additional layer masks to filter which entity types
     * can collide with each other, enabling fine-grained control over collision pairs.
     *
     * @see CollisionComponent
     * @see AabbColliderComponent
     * @see TriggerCollisionEvent
     * @see SolidCollisionEvent
     *
     * @see [Eri05, Chapter 7]
     */
    class GridCollisionDetectionSystem : public System {

        /**
         * @brief Hash functor for pairs of GUIDs.
         *
         * Enables the use of GUID pairs as keys in unordered containers. The hash combines
         * both GUIDs using XOR with a bit-shift to reduce collision probability for symmetric pairs.
         *
         * @todo switch to uint32_t once helios/#174 is implemented
         */
        struct GuidPairHash {

            /**
             * @brief Computes a hash value for a GUID pair.
             *
             * @param pair The pair of GUIDs to hash.
             *
             * @return A combined hash value for both GUIDs.
             */
            std::size_t operator()(const std::pair<helios::util::Guid, helios::util::Guid>& pair) const {

                auto g1 = std::hash<helios::util::Guid>{}(pair.first);
                auto g2 = std::hash<helios::util::Guid>{}(pair.second);

                // compute the hash for the pair - shift g2 one position left, then xor with g1.
                return g1 ^ (g2 << 1);
            };

        };

        /**
        * @brief Internal structure holding references to a potential collision candidate.
        *
        * Bundles the GameObject pointer with its collision-relevant components for
        * efficient access during the narrow phase.
        */
        struct CollisionCandidate {

            /**
             * @brief Pointer to the GameObject entity.
             */
            helios::engine::game::GameObject* gameObject;

            /**
             * @brief Pointer to the AABB collider component providing world-space bounds.
             */
            helios::engine::game::components::physics::AabbColliderComponent* aabbColliderComponent;

            /**
             * @brief Pointer to the collision component defining layer masks and collision behavior.
             */
            helios::engine::game::components::physics::CollisionComponent* collisionComponent;
        };

        /**
         * @brief Represents a single cell in the spatial partitioning grid.
         *
         * Each cell stores references to all collision candidates whose AABBs overlap
         * this cell's spatial region.
         */
        struct GridCell {

            /**
             * @brief List of collision candidates currently occupying this cell.
             */
            std::vector<CollisionCandidate> collisionCandidates;

            /**
             * @brief Clears all candidates from this cell.
             */
            void clear() {
                collisionCandidates.clear();
            }
        };

        /**
         * @brief Type alias for min/max bounds as unsigned integer pair.
         */
        using BoundsMinMax = std::pair<unsigned int, unsigned int>;

        /**
         * @brief Set of already-processed collision pairs to avoid duplicate events.
         *
         * Stores pairs of GUIDs in canonical order (smaller GUID first) to ensure each
         * collision pair is processed only once per frame, even when entities span multiple cells.
         */
        std::unordered_set<std::pair<helios::util::Guid, helios::util::Guid>, GuidPairHash> solvedCollisions_;

        /**
         * @brief Size of each grid cell in world units.
         */
        float cellSize_;

        /**
         * @brief World-space bounds defining the spatial region covered by the grid.
         */
        helios::math::aabbf gridBounds_;

        /**
         * @brief Flat storage for all grid cells, indexed as x + y * cellsX + z * (cellsX * cellsY).
         */
        std::vector<GridCell> cells_;

        /**
         * @brief Number of cells along the X axis.
         */
        unsigned int cellsX_;

        /**
         * @brief Number of cells along the Y axis.
         */
        unsigned int cellsY_;

        /**
         * @brief Number of cells along the Z axis.
         */
        unsigned int cellsZ_;

        /**
         * @brief Initializes the grid based on world bounds and cell size.
         *
         * Computes the number of cells needed in each dimension and allocates
         * the cell storage.
         */
        void initGrid() {

            helios::math::vec3f size = gridBounds_.size();

            cellsX_ = std::max(1, static_cast<int>(std::ceil(size[0] / cellSize_)));
            cellsY_ = std::max(1, static_cast<int>(std::ceil(size[1] / cellSize_)));
            cellsZ_ = std::max(1, static_cast<int>(std::ceil(size[2] / cellSize_)));

            cells_.resize(cellsX_ * cellsY_ * cellsZ_);
        }

        /**
         * @brief Prepares the grid for a new collision detection pass.
         *
         * Clears all collision candidates from every cell and resets the set of
         * already-solved collision pairs.
         */
        inline void prepareCollisionDetection() {
            for (auto& cell : cells_) {
                cell.clear();
            }

            solvedCollisions_.clear();
        }

        /**
         * @brief Posts collision events to the UpdateContext's event sink.
         *
         * Publishes TriggerCollisionEvent and/or SolidCollisionEvent to the UpdateContext
         * based on the collision type and reporter flags. An entity marked as
         * collision reporter receives the event as the "source" entity.
         *
         * @param candidate First collision candidate (potential event source).
         * @param match Second collision candidate (collision partner).
         * @param contact The contact point between the two AABBs.
         * @param aIsCollisionReporter True if candidate reports collisions.
         * @param bIsCollisionReporter True if match reports collisions.
         * @param isSolidCollision True if this is a solid (physical) collision.
         * @param isTriggerCollision True if this is a trigger (gameplay) collision.
         * @param updateContext Context for pushing events to the event queue.
         */
        void postEvent(
            helios::engine::game::GameObject* candidate,
            helios::engine::game::GameObject* match,
            helios::math::vec3f contact,
            bool aIsCollisionReporter,
            bool bIsCollisionReporter,
            bool isSolidCollision,
            bool isTriggerCollision,
            helios::engine::game::UpdateContext& updateContext
        ) const noexcept {

            assert((isSolidCollision || isTriggerCollision)
                && (aIsCollisionReporter || bIsCollisionReporter)
                && "Preconditions not matched for postEvent.");

            // post the events
            if (isTriggerCollision) {
                if (aIsCollisionReporter) {
                    updateContext.pushEvent<helios::engine::game::event::TriggerCollisionEvent>(
                        candidate, match, contact
                    );
                } else if (bIsCollisionReporter) {
                    updateContext.pushEvent<helios::engine::game::event::TriggerCollisionEvent>(
                        match, candidate, contact
                    );
                }
            }

            if (isSolidCollision) {
                if (aIsCollisionReporter) {
                    updateContext.pushEvent<helios::engine::game::event::SolidCollisionEvent>(
                        candidate, match, contact
                    );
                } else if (bIsCollisionReporter) {
                    updateContext.pushEvent<helios::engine::game::event::SolidCollisionEvent>(
                        match, candidate, contact
                    );
                }
            }

        }

        /**
         * @brief Determines the collision type between two entities based on their layer masks.
         *
         * Evaluates both solid and trigger collision masks to determine if and how two entities
         * can collide. Solid collisions are symmetric (both entities must accept collision with
         * each other), while trigger collisions are asymmetric (either entity can trigger).
         *
         * @param cc Collision component of the first entity.
         * @param matchCC Collision component of the second entity.
         * @param isSolidCollision Output: set to true if a solid collision is detected.
         * @param isTriggerCollision Output: set to true if a trigger collision is detected.
         * @param aIsCollisionReporter Output: true if the first entity reports collisions.
         * @param bIsCollisionReporter Output: true if the second entity reports collisions.
         */
        void findCollisionType(
            const helios::engine::game::components::physics::CollisionComponent* cc,
            const helios::engine::game::components::physics::CollisionComponent* matchCC,
            bool& isSolidCollision,
            bool& isTriggerCollision,
            bool& aIsCollisionReporter,
            bool& bIsCollisionReporter
            ) const noexcept {

            isSolidCollision   = false;
            isTriggerCollision = false;

            aIsCollisionReporter = cc->isCollisionReporter();
            bIsCollisionReporter = matchCC->isCollisionReporter();

            // none of the reports a collision as a pair? skip!
            if (!aIsCollisionReporter && !bIsCollisionReporter) {
                return;
            }

            bool aCanSolidCollideWithB = (cc->solidCollisionMask() & matchCC->layerId()) != 0;
            bool bCanSolidCollideWithA = (matchCC->solidCollisionMask() & cc->layerId()) != 0;

            bool aCanTriggerCollideWithB = (cc->triggerCollisionMask() & matchCC->layerId()) != 0;
            bool bCanTriggerCollideWithA = (matchCC->triggerCollisionMask() & cc->layerId()) != 0;

            // solid collision is treated symmetric
            isSolidCollision = aCanSolidCollideWithB && bCanSolidCollideWithA;
            // trigger collision is treated asymmetric
            isTriggerCollision = aCanTriggerCollideWithB || bCanTriggerCollideWithA;

        }

    public:

        /**
         * @brief Constructs a GridCollisionDetectionSystem with specified bounds and cell size.
         *
         * The grid is initialized to cover the given world bounds. Cell size determines
         * the granularity of spatial partitioning; smaller cells reduce false positives
         * but increase memory and insertion overhead.
         *
         * @param bounds World-space AABB defining the region where collision detection is active.
         * @param cellSize Size of each grid cell in world units. Must be greater than zero.
         *
         * @note Given n objects, the 1/3 rule should be applied for determining the number of cells for partitioning,
         * i.e. with n objects, the cube should consist of k x k x k cells, with k = n^{1/3} - that is, on average,
         * one cell fits one object that should be tested for collision. Dimensions of said cells
         * are derived by taking the reference space into account, e.g. WorldBounds.size/k (see [HS99]).
         */
        explicit GridCollisionDetectionSystem(
            const helios::math::aabbf& bounds,
            const float cellSize
        ) : gridBounds_(bounds),
            cellSize_(cellSize) {
            assert(cellSize_ > 0.0f && "cellSize must not be <= 0.0f");
            initGrid();
        }

        /**
         * @brief Performs collision detection for all active entities.
         *
         * Executes the complete collision detection pipeline each frame:
         * 1. Clears all grid cells and resets solved collision pairs.
         * 2. Iterates all active GameObjects with CollisionComponent and AabbColliderComponent.
         * 3. Inserts each entity into all grid cells its AABB overlaps.
         * 4. For each cell with multiple candidates, runs narrow-phase AABB intersection tests.
         * 5. Publishes collision events (TriggerCollisionEvent, SolidCollisionEvent) to the event queue.
         *
         * @param updateContext The update context providing access to GameWorld and event queue.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            prepareCollisionDetection();

            // only consider enabled CollisionComponents
            constexpr auto filter = helios::engine::game::GameObjectFilter::Active | helios::engine::game::GameObjectFilter::ComponentEnabled;

            for (auto [entity, cc, acc] : gameWorld_->find<
                helios::engine::game::components::physics::CollisionComponent,
                helios::engine::game::components::physics::AabbColliderComponent

            >(filter).each()) {

                if (!acc->boundsInitialized()) {
                    continue;
                }

                if (!cc->isEnabled() || (cc->triggerCollisionMask() == 0 && cc->solidCollisionMask() == 0)) {
                    continue;
                }

                const auto& worldBounds = acc->bounds();

                updateCollisionCandidate(
                    entity,
                    worldBoundsToGridBounds(worldBounds),
                    acc,
                    cc
                );
            }

            for (auto& cell : cells_) {

                if (cell.collisionCandidates.size() < 2) {
                    continue;
                }

                solveCell(cell, updateContext);
            }
        }


        /**
         * @brief Converts world-space AABB bounds to grid cell indices.
         *
         * Transforms a floating-point AABB in world space to integer cell coordinates,
         * clamped to the valid grid range. Used to determine which cells an entity occupies.
         *
         * @param aabbf The world-space AABB to convert.
         *
         * @return An integer AABB representing the range of grid cell indices the entity spans.
         */
        [[nodiscard]] helios::math::aabbi worldBoundsToGridBounds(const helios::math::aabbf& aabbf) const noexcept {

            helios::math::vec3f min = aabbf.min() - gridBounds_.min();
            helios::math::vec3f max = aabbf.max() - gridBounds_.min();

            int xMin = static_cast<int>(std::floor(min[0] / cellSize_));
            int yMin = static_cast<int>(std::floor(min[1] / cellSize_));
            int zMin = static_cast<int>(std::floor(min[2] / cellSize_));

            int xMax = static_cast<int>(std::floor(max[0] / cellSize_));
            int yMax = static_cast<int>(std::floor(max[1] / cellSize_));
            int zMax = static_cast<int>(std::floor(max[2] / cellSize_));

            return helios::math::aabbi{
                std::clamp(xMin, 0, static_cast<int>(cellsX_ -1)),
                std::clamp(yMin, 0, static_cast<int>(cellsY_ -1)),
                std::clamp(zMin, 0, static_cast<int>(cellsZ_ -1)),
                std::clamp(xMax, 0, static_cast<int>(cellsX_ -1)),
                std::clamp(yMax, 0, static_cast<int>(cellsY_ -1)),
                std::clamp(zMax, 0, static_cast<int>(cellsZ_ -1)),
            };
        }


        /**
         * @brief Inserts a collision candidate into all grid cells it overlaps.
         *
         * Iterates through all cells within the given bounds and adds the candidate
         * to each cell's collision candidate list for subsequent narrow-phase testing.
         *
         * @param go Pointer to the GameObject entity.
         * @param bounds Grid cell index bounds (integer AABB) the entity spans.
         * @param aabbColliderComponent Pointer to the entity's AABB collider component.
         * @param collisionComponent Pointer to the entity's collision component.
         */
        void updateCollisionCandidate(
            helios::engine::game::GameObject* go,
            const helios::math::aabbi& bounds,
            helios::engine::game::components::physics::AabbColliderComponent* aabbColliderComponent,
            helios::engine::game::components::physics::CollisionComponent* collisionComponent
        ) {
            const auto xMin = bounds.min()[0];
            const auto xMax = bounds.max()[0];
            const auto yMin = bounds.min()[1];
            const auto yMax = bounds.max()[1];
            const auto zMin = bounds.min()[2];
            const auto zMax = bounds.max()[2];

            for (int x = xMin; x <  xMax + 1; x++) {
                for (int y = yMin; y <  yMax + 1; y++) {
                    for (int z = zMin; z <  zMax + 1; z++) {
                        auto& gridCell = cell(x, y, z);

                       gridCell.collisionCandidates.push_back(
                            CollisionCandidate{
                                go,
                                aabbColliderComponent,
                                collisionComponent
                            }
                        );
                    }
                }
            }

        }


        /**
         * @brief Performs narrow-phase collision detection for all candidates in a cell.
         *
         * Tests all unique pairs of collision candidates within the cell using AABB intersection.
         * For each collision detected, determines collision type (solid/trigger) based on layer
         * masks and publishes appropriate events. Uses a solved-set to avoid duplicate events
         * when entities span multiple cells.
         *
         * @param cell The grid cell containing collision candidates to test.
         * @param updateContext Context for pushing collision events to the event queue.
         */
        void solveCell(GridCell& cell, helios::engine::game::UpdateContext& updateContext) {

            auto& candidates = cell.collisionCandidates;

            // we will skip this cell if none of the candidates reports a collision
            bool isCollisionReporter = false;
            for (auto& candidate : candidates) {
                if (candidate.collisionComponent->isCollisionReporter()) {
                    isCollisionReporter = true;
                    break;
                }
            }

            if (!isCollisionReporter) {
                return;
            }

            for (size_t i = 0; i < candidates.size(); i++) {

                CollisionCandidate& candidate = candidates[i];
                helios::engine::game::components::physics::CollisionComponent* cc = candidate.collisionComponent;

                const helios::math::aabbf& aabbCandidate = candidate.aabbColliderComponent->bounds();

                for (size_t j = i+1; j < candidates.size(); j++) {

                    CollisionCandidate& match = candidates[j];

                    helios::engine::game::components::physics::CollisionComponent* matchCC = match.collisionComponent;

                    bool isSolidCollision = false;
                    bool isTriggerCollision = false;
                    bool aIsCollisionReporter = false;
                    bool bIsCollisionReporter = false;

                    findCollisionType(
                        cc, matchCC,
                        isSolidCollision, isTriggerCollision,
                        aIsCollisionReporter, bIsCollisionReporter);

                    if ((!isSolidCollision && !isTriggerCollision) ||
                        (!aIsCollisionReporter && !bIsCollisionReporter)) {
                        continue;
                    }

                    const helios::math::aabbf& aabbMatch = match.aabbColliderComponent->bounds();
                    if (!aabbCandidate.intersects(aabbMatch)) {
                        continue;
                    }

                    auto lGuid = candidate.gameObject->guid();
                    auto rGuid = match.gameObject->guid();

                    if (lGuid > rGuid) {
                        std::swap(lGuid, rGuid);
                    }

                    if (solvedCollisions_.contains({lGuid, rGuid})) {
                        continue;
                    }

                    solvedCollisions_.insert({lGuid, rGuid});

                    postEvent(
                        candidate.gameObject, match.gameObject,
                        helios::math::overlapCenter(aabbCandidate, aabbMatch),
                        aIsCollisionReporter, bIsCollisionReporter,
                        isSolidCollision, isTriggerCollision,
                        updateContext
                    );
                }
            }
        }

        /**
         * @brief Accesses a grid cell by its 3D coordinates.
         *
         * @param x X-coordinate of the cell (0 to cellsX - 1).
         * @param y Y-coordinate of the cell (0 to cellsY - 1).
         * @param z Z-coordinate of the cell (0 to cellsZ - 1).
         *
         * @return Reference to the GridCell at the specified coordinates.
         */
        [[nodiscard]] GridCell& cell(unsigned int x, unsigned int y, unsigned int z) noexcept {
            assert (x < cellsX_ && y < cellsY_ && z < cellsZ_ && "Invalid range values");

            return cells_[
                x + y * cellsX_ + z * (cellsX_ * cellsY_)
            ];
        }

        /**
         * @brief Returns the number of cells along the X axis.
         *
         * @return Number of grid cells in the X dimension.
         */
        [[nodiscard]] unsigned int cellsX() const noexcept {
            return cellsX_;
        }

        /**
         * @brief Returns the number of cells along the Y axis.
         *
         * @return Number of grid cells in the Y dimension.
         */
        [[nodiscard]] unsigned int cellsY() const noexcept {
            return cellsY_;
        }

        /**
         * @brief Returns the number of cells along the Z axis.
         *
         * @return Number of grid cells in the Z dimension.
         */
        [[nodiscard]] unsigned int cellsZ() const noexcept {
            return cellsZ_;
        }

    };


}