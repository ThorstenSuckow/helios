/**
 * @file GridCollisionDetectionSystem.ixx
 * @brief Spatial-partitioning based collision detection using a uniform 3D grid.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <helios/helios_config.h>
#include <unordered_set>
#include <vector>


export module helios.engine.game.physics.collision.systems.GridCollisionDetectionSystem;

import helios.engine.ecs.System;
import helios.engine.ecs.UpdateContext;
import helios.engine.core.data.GameObjectFilter;
import helios.engine.ecs.GameObject;
import helios.engine.ecs.GameWorld;


import helios.engine.game.physics.collision.events.TriggerCollisionEvent;
import helios.engine.game.physics.collision.events.SolidCollisionEvent;

import helios.engine.game.physics.collision.components.CollisionComponent;
import helios.engine.game.physics.collision.components.CollisionStateComponent;
import helios.engine.game.physics.collision.components.AabbColliderComponent;

import helios.engine.game.physics.collision.types.CollisionBehavior;

import helios.util.Guid;
import helios.math;

import helios.util.log;

using namespace helios::engine::game::physics::collision::components;
using namespace helios::engine::game::physics::collision::events;

#define HELIOS_LOG_SCOPE "helios::engine::game::physics::systems::GridCollisionDetectionSystem"
export namespace helios::engine::game::physics::collision::systems {

    /**
     * @brief Collision detection system using uniform spatial partitioning for Broadphase and
     * AABB overlaps in the Narrowphase.
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
    class GridCollisionDetectionSystem : public helios::engine::ecs::System {

        /**
         * @brief Helper-struct representing the properties and interaction state of a collision
         * event between two entities.
         */
        struct CollisionStruct {
            bool isSolidCollision = false;
            bool isTriggerCollision = false;
            bool aIsCollisionReporter = false;
            bool bIsCollisionReporter = false;
            helios::engine::game::physics::collision::types::CollisionBehavior aCollisionBehavior;
            helios::engine::game::physics::collision::types::CollisionBehavior bCollisionBehavior;

            [[nodiscard]] inline constexpr bool hasAnyInteraction() const noexcept {
                return (isSolidCollision || isTriggerCollision) && (aIsCollisionReporter || bIsCollisionReporter);
            }
        };

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
            helios::engine::ecs::GameObject* gameObject;

            /**
             * @brief Pointer to the AABB collider component providing world-space bounds.
             */
            AabbColliderComponent* aabbColliderComponent;

            /**
             * @brief Pointer to the collision component defining layer masks and collision behavior.
             */
            CollisionComponent* collisionComponent;

            /**
             * @brief Pointer to the collision state component for storing collision results.
             */
            CollisionStateComponent* collisionStateComponent;
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
         * @brief Scoped logger instance for structured logging within the current context.
         */
        static inline const auto& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

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
         * @brief Helper to keep track of updated cells (with potential collisions candidates) in one pass.
         */
        std::vector<size_t> trackedCells_;

        /**
         * @brief Initializes the grid based on world bounds and cell size.
         *
         * Computes the number of cells needed in each dimension and allocates
         * the cell storage.
         */
        void initGrid() {

            helios::math::vec3f size = gridBounds_.size();

            cellsX_ = std::max(1u, static_cast<unsigned int>(std::ceil(size[0] / cellSize_)));
            cellsY_ = std::max(1u, static_cast<unsigned int>(std::ceil(size[1] / cellSize_)));
            cellsZ_ = std::max(1u, static_cast<unsigned int>(std::ceil(size[2] / cellSize_)));

            const size_t cellCount = static_cast<size_t>(cellsX_) * cellsY_ * cellsZ_;

            // this would make 100'000'000 * sizeof(GridCell) Bytes
            // if we have 24 Bytes per GridCell, we end up with 2,4 GB alone for this spatial grid.
            if (cellCount > 100'000'000) {
                logger_.warn(std::format("Spatial Grid requested {0} cells", cellCount));
                throw std::runtime_error("Cell count too high.");
            }

            trackedCells_.reserve(cellCount);
            cells_.resize(cellCount);
        }

        /**
         * @brief Prepares the grid for a new collision detection pass.
         *
         * Clears all collision candidates from every cell and resets the set of
         * already-solved collision pairs.
         */
        inline void prepareCollisionDetection() {
            for (const auto idx : trackedCells_) {
                cells_[idx].clear();
            }

            trackedCells_.clear();
            solvedCollisions_.clear();
        }

        /**
         * @brief Posts collision events to the UpdateContext's event sink.
         *
         * @details Updates the CollisionStateComponent of both entities with collision
         * information including contact point, collision type, behavior, and reporter status.
         * An entity marked as collision reporter receives the event as the "source" entity.
         *
         * @param candidate First collision candidate (potential event source).
         * @param match Second collision candidate (collision partner).
         * @param contact The contact point between the two AABBs.
         * @param collisionStruct Struct containing collision type and behavior information.
         * @param updateContext Context for pushing events to the event queue.
         * @param csc_a Collision state component of the first entity.
         * @param csc_b Collision state component of the second entity.
         */
        inline void postEvent(
            const helios::engine::ecs::GameObject* candidate,
            const helios::engine::ecs::GameObject* match,
            const helios::math::vec3f contact,
            const CollisionStruct collisionStruct,
            const helios::engine::ecs::UpdateContext& updateContext,
            CollisionStateComponent* csc_a,
            CollisionStateComponent* csc_b
        ) const noexcept {

            bool aIsCollisionReporter = collisionStruct.aIsCollisionReporter;
            bool bIsCollisionReporter = collisionStruct.bIsCollisionReporter;
            bool isSolidCollision     = collisionStruct.isSolidCollision;
            bool isTriggerCollision   = collisionStruct.isTriggerCollision;

            assert((isSolidCollision || isTriggerCollision)
                && (aIsCollisionReporter || bIsCollisionReporter)
                && "Preconditions not matched for postEvent.");

            // post the events
            if (isTriggerCollision || isSolidCollision) {
                csc_a->setState(
                    contact, isSolidCollision, isTriggerCollision, collisionStruct.aCollisionBehavior,
                    aIsCollisionReporter, match->guid()
                );
                csc_b->setState(
                    contact, isSolidCollision, isTriggerCollision, collisionStruct.bCollisionBehavior,
                    bIsCollisionReporter, candidate->guid()
                );
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
         *
         * @return CollisionStruct a struct with the requested collision information.
         */
        [[nodiscard]] inline CollisionStruct findCollisionType(
            const CollisionComponent* cc,
            const CollisionComponent* matchCC
        ) const noexcept {

            auto isSolidCollision   = false;
            auto isTriggerCollision = false;

            auto aIsCollisionReporter = cc->isCollisionReporter();
            auto bIsCollisionReporter = matchCC->isCollisionReporter();

            // none of the reports a collision as a pair? skip!
            if (!aIsCollisionReporter && !bIsCollisionReporter) {
                return CollisionStruct{};
            }

            bool aCanSolidCollideWithB = (cc->solidCollisionMask() & matchCC->layerId()) != 0;
            bool bCanSolidCollideWithA = (matchCC->solidCollisionMask() & cc->layerId()) != 0;

            bool aCanTriggerCollideWithB = (cc->triggerCollisionMask() & matchCC->layerId()) != 0;
            bool bCanTriggerCollideWithA = (matchCC->triggerCollisionMask() & cc->layerId()) != 0;

            // solid collision is treated symmetric
            isSolidCollision = aCanSolidCollideWithB && bCanSolidCollideWithA;

            #if HELIOS_DEBUG
            // detect asymmetric solid collision
            if (aCanSolidCollideWithB && !bCanSolidCollideWithA) {
                logger_.warn("Collision Asymmetry detected!");
            }
            #endif

            // trigger collision is treated asymmetric
            isTriggerCollision = aCanTriggerCollideWithB || bCanTriggerCollideWithA;

            return CollisionStruct {
                isSolidCollision,
                isTriggerCollision,
                aIsCollisionReporter,
                bIsCollisionReporter,
                isSolidCollision
                ? cc->solidCollisionBehavior(matchCC->layerId()) : cc->triggerCollisionBehavior(matchCC->layerId()),
                isSolidCollision
                ? matchCC->solidCollisionBehavior(cc->layerId()) : matchCC->triggerCollisionBehavior(cc->layerId())
            };
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
        void update(helios::engine::ecs::UpdateContext& updateContext) noexcept override {

            prepareCollisionDetection();

            // only consider enabled CollisionSettingsComponents
            constexpr auto filter = helios::engine::core::data::GameObjectFilter::Active | helios::engine::core::data::GameObjectFilter::ComponentEnabled;

            for (auto [entity, cc, csc, acc] : gameWorld_->find<
                CollisionComponent,
                CollisionStateComponent,
                AabbColliderComponent

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
                    cc,
                    csc
                );
            }

            for (const auto idx : trackedCells_) {

                if (cells_[idx].collisionCandidates.size() < 2) {
                    continue;
                }

                solveCell(cells_[idx], updateContext);
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
                std::clamp(xMin, 0, static_cast<int>(cellsX_ - 1)),
                std::clamp(yMin, 0, static_cast<int>(cellsY_ - 1)),
                std::clamp(zMin, 0, static_cast<int>(cellsZ_ - 1)),
                std::clamp(xMax, 0, static_cast<int>(cellsX_ - 1)),
                std::clamp(yMax, 0, static_cast<int>(cellsY_ - 1)),
                std::clamp(zMax, 0, static_cast<int>(cellsZ_ - 1)),
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
        inline void updateCollisionCandidate(
            helios::engine::ecs::GameObject* go,
            const helios::math::aabbi& bounds,
            AabbColliderComponent* aabbColliderComponent,
            CollisionComponent* collisionComponent,
            CollisionStateComponent* collisionStateComponent
        ) {
            const auto xMin = bounds.min()[0];
            const auto xMax = bounds.max()[0];
            const auto yMin = bounds.min()[1];
            const auto yMax = bounds.max()[1];
            const auto zMin = bounds.min()[2];
            const auto zMax = bounds.max()[2];

            for (int x = xMin; x <= xMax; x++) {
                for (int y = yMin; y <= yMax; y++) {
                    for (int z = zMin; z <= zMax; z++) {
                        auto& [collisionCandidates] = cell(x, y, z);

                        collisionCandidates.push_back(
                            CollisionCandidate{
                                go,
                                aabbColliderComponent,
                                collisionComponent,
                                collisionStateComponent
                            }
                        );

                        // only consider the first added to prevent dups
                        if (collisionCandidates.size() == 1) {
                            const auto idx = cellIndex(x, y, z);
                            trackedCells_.push_back(idx);
                        }
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
        inline void solveCell(GridCell& cell, helios::engine::ecs::UpdateContext& updateContext) {

            auto& candidates = cell.collisionCandidates;

            // we will skip this cell if none of the candidates reports a collision
            bool isCollisionReporter = false;
            for (const auto& candidate : candidates) {
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
                CollisionComponent* cc = candidate.collisionComponent;
                CollisionStateComponent* csc = candidate.collisionStateComponent;

                const helios::math::aabbf& aabbCandidate = candidate.aabbColliderComponent->bounds();

                for (size_t j = i+1; j < candidates.size(); j++) {

                    auto& [gameObject, aabbColliderComponent, collisionComponent, collisionStateComponent] = candidates[j];

                    CollisionComponent* matchCC       = collisionComponent;
                    CollisionStateComponent* matchCSC = collisionStateComponent;

                    const auto collisionStruct = findCollisionType(cc, matchCC);

                    if (!collisionStruct.hasAnyInteraction()) {
                        continue;
                    }

                    const helios::math::aabbf& aabbMatch = aabbColliderComponent->bounds();
                    if (!aabbCandidate.intersects(aabbMatch)) {
                        continue;
                    }

                    auto lGuid = candidate.gameObject->guid();
                    auto rGuid = gameObject->guid();

                    if (lGuid > rGuid) {
                        std::swap(lGuid, rGuid);
                    }

                    // if we have already processed a collision, do not add this collision again.
                    if (solvedCollisions_.contains({lGuid, rGuid})) {
                        continue;
                    }

                    solvedCollisions_.insert({lGuid, rGuid});

                    postEvent(
                        candidate.gameObject, gameObject,
                        helios::math::overlapCenter(aabbCandidate, aabbMatch),
                        collisionStruct,
                        updateContext, csc, matchCSC
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
        [[nodiscard]] inline GridCell& cell(const unsigned int x, const  unsigned int y, const unsigned int z) noexcept {
            return cells_[cellIndex(x, y, z)];
        }

        /**
         * @brief Computes the 1D index of a cell in a 3D grid based on its x, y, and z coordinates.
         *
         * @param x The x-coordinate of the cell (must be less than the grid's x-dimension).
         * @param y The y-coordinate of the cell (must be less than the grid's y-dimension).
         * @param z The z-coordinate of the cell (must be less than the grid's z-dimension).
         *
         * @return The 1D index of the cell in the grid.
         */
        [[nodiscard]] inline constexpr size_t cellIndex(const unsigned int x, const unsigned int y, const unsigned int z) const noexcept {
            assert (x < cellsX_ && y < cellsY_ && z < cellsZ_ && "Invalid range values");

            return x + y * cellsX_ + (z * cellsX_ * cellsY_);
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
