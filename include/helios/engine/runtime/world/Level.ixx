/**
 * @file Level.ixx
 * @brief Defines the Level class representing a game level or stage.
 */
module;

export module helios.engine.runtime.world.Level;

import helios.math.types;
import helios.core.units.Unit;
import helios.scene.SceneNode;


export namespace helios::engine::runtime::world {

    /**
     * @brief Represents a game level containing the scene graph and world boundaries.
     *
     * @details The Level class acts as a container for the scene hierarchy (via a root node)
     * and defines the spatial limits of the game world. It handles unit conversion for
     * boundaries to ensure consistent internal representation (meters).
     */
    class Level {

        /**
         * @brief The root node of the scene graph associated with this level.
         */
        helios::scene::SceneNode* rootNode_ = nullptr;

        /**
         * @brief The spatial boundaries of the level in engine units (meters).
         */
        helios::math::aabbf bounds_;
    public:

        /**
         * @brief Constructs a new Level with a specific root scene node.
         *
         * @param rootNode Pointer to the root node of the scene graph for this level.
         *                 The Level does not take ownership of this pointer.
         */
        explicit Level(helios::scene::SceneNode* rootNode) : rootNode_(rootNode) {}

        /**
         * @brief Sets the spatial boundaries of the level.
         *
         * @details Converts the provided bounds from the specified unit to the engine's
         * standard unit (meters) before storing them.
         *
         * @param bounds The axis-aligned bounding box defining the level limits.
         * @param unit The unit of measurement for the provided bounds (defaults to Meter).
         */
        void setBounds(
            const helios::math::aabbf& bounds,
            const  helios::core::units::Unit unit = helios::core::units::Unit::Meter) noexcept {

            bounds_ = helios::core::units::from(bounds, unit);

        }

        /**
         * @brief Retrieves the root scene node of the level.
         *
         * @return Pointer to the root SceneNode.
         */
        [[nodiscard]] helios::scene::SceneNode* rootNode() const noexcept {
            return rootNode_;
        }

        /**
         * @brief Retrieves the level boundaries in engine units (meters).
         *
         * @return The axis-aligned bounding box of the level.
         */
        [[nodiscard]] const helios::math::aabbf& bounds() const noexcept {
            return bounds_;
        }

    };

}


