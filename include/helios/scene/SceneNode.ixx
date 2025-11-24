/**
 * @file SceneNode.ixx
 *
 * @brief Scene graph node representing a transform and optional renderable
 */
module;

#include <memory>
#include <vector>

export module helios.scene.SceneNode;

import :SceneFwd;

import helios.rendering.Renderable;
import helios.util.Guid;
import helios.scene.Transform;
import helios.math.types;

export namespace helios::scene {


    /**
     * @brief SceneGraphKey as passkey idiom for accessing `setWorldTransform()`.
     * All friend classes are able to construct the SceneGraphKey for accessing
     * pass key guarded methods like `setWorldTransform`.
     *
     * @see `setWorldTransform`
     *
     * @see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p2893r3.html#passkey-idiom
     */
    struct SceneGraphKey {
    private:
        friend class helios::scene::Scene;
        SceneGraphKey() = default;
    };

    /**
     * @brief Represents a SceneNode within a SceneGraph.
     *
     * A SceneNode holds its local transformation and a (shared) pointer to
     * a Renderable object (the representative of a visible element).
     * Furthermore, a SceneNode can have multiple child nodes uniquely owned by
     * **this** SceneNode.
     *
     * A SceneNode holds a global unique identifier and tracks a dirty state (`needsUpdate_`)
     * related to any transformations that occurred on the SceneNode.
     * Ownership is managed by a collection of unique pointers of the child nodes.
     * SceneNodes are designed to be not copyable and not assignable to prevent duplication
     * and to enforce the hierarchical tree structure.
     *
     * The `worldTransform_` is derived from a SceneNode's `localTransform_` and the SceneNode's
     * parent's `worldTransform`s. A call to `worldTransform()` will update the particular subtree
     * of the SceneNode bottom-up to make sure the actual values are available with the SceneNode's
     * `worldTransform`. A Scene might also propagate changes top-bottom - see `Scene:.udpateNodes()`.
     *
     */
    class SceneNode {

        private:
        /**
         * @brief A unique identifier for this SceneNode, computed automatically
         * during instantiation.
         */
        const util::Guid guid_;

        protected:
        /**
         * @brief Boolean flag to represent the dirty-state of this node.
         * Should be set to false if the current `worldTransform_`
         * for this node was computed, otherwise true, e.g. if the node's
         * local transform has been changed.
         */
        bool needsUpdate_ = true;

        /**
         * @brief A shared pointer to the Renderable this node represents in the
         * SceneGraph. May be nullptr if this node does not represent a
         * renderable object.
         */
        std::shared_ptr<helios::rendering::Renderable> renderable_;

        /**
         * @brief The parent node of **this** node.
         * This will be nullptr for any root node. Implementing APIs must
         * take care of properly assigning parent nodes when adding
         * child nodes.
         */
        SceneNode* parent_ = nullptr;

        /**
         * @brief A list of unique pointers to this node's children.
         * Ensures ownership of the child nodes withing the graph hierarchy.
         */
        std::vector<std::unique_ptr<SceneNode>> children_;

        /**
         * @brief The 4x4 worldTransform-Matrix of this node.
         * This matrix is initially set to the identity matrix.
         * Implementing APIs MUST make sure to recompute the worldTransform
         * before each rendering pass.
         */
        math::mat4f worldTransform_ = math::mat4f::identity();

        /**
         * @brief A local transformation representing affine transformation
         * information for this SceneNode. Encapsulates rotation, scaling and
         * translation.
         */
        Transform localTransform_;

        /**
         * @brief Sets the parent of this SceneNode.
         * Internally used once a child was added to this SceneNode.
         *
         * @param parentNode The parent of this SceneNode
         *
         * @see addChild()
         */
        void setParent(SceneNode* parentNode);

        public:
            virtual ~SceneNode() = default;

            /**
             * @brief Delete copy constructor.
             * A SceneNode is not copyable.
             */
            SceneNode(const SceneNode &) = delete;

            /**
             * @brief Delete copy assignment constructor.
             * A SceneNode ist not indtended to be copied.
             */
            SceneNode& operator=(const SceneNode&) = delete;

            /**
             * @todo explicitly implement move (assignment) constructor if required,
             * since we have deleted the copy constructors
             */

            /**
             * @brief Constructs a new SceneNode that represents no renderable object.
             * Nodes constructed in this way should be treated as transformation
             * nodes.
             */
            SceneNode() noexcept;

            /**
             * @brief Constructs a new SceneNode that represents no renderable object.
             * Nodes constructed in this way should be treated as transformation
             * nodes.
             *
             * @param transform The initial transformation for this node.
             */
            explicit SceneNode(const Transform& transform) noexcept;

            /**
             * @brief Constructs a new SceneNode representing a renderable object.
             *
             * @param renderable A shared_ptr to the Renderable this SceneNode
             * represents in a Scene.
             * @param transform The initial local transform for this node.
             */
            explicit SceneNode(
                std::shared_ptr<helios::rendering::Renderable> renderable,
                const Transform& transform
            ) noexcept;

            /**
             * @brief Constructs a new SceneNode representing a renderable object.
             *
             * @param renderable A shared_ptr to the Renderable this SceneNode
             * represents in a Scene.
             */
            explicit SceneNode(
                std::shared_ptr<helios::rendering::Renderable> renderable
            ) noexcept;

            /**
             * @brief Returns the globally unique identifier for this SceneNode.
             *
             * @return The unique identifier for this SceneNode.
             */
            [[nodiscard]] const helios::util::Guid& guid() const noexcept;

            /**
             * @brief Adds a new child node to this SceneNode.
             * The node's parent is automatically set to **this** node
             *
             * @param sceneNode The node to add as a child.
             *
             * @return The raw pointer to the newly added node, or nullptr if
             * adding failed
             */
            [[nodiscard]] virtual SceneNode* addChild(std::unique_ptr<SceneNode> sceneNode);

            /**
             * @brief Returns a const ref to the list of this node's children.
             *
             * @return A const ref to the list of children of this node.
             */
            [[nodiscard]] const std::vector<std::unique_ptr<SceneNode>>& children() const noexcept;

            /**
             * @brief Returns a shared pointer to the Renderable of this SceneNode.
             *
             * @return A shared_ptr to the Renderable, may be nullptr if none is set.
             */
            [[nodiscard]] std::shared_ptr<const helios::rendering::Renderable> renderable() const noexcept;

            /**
             * @brief Returns a shared pointer to the non-const Renderable of this SceneNode.
             *
             * @return A shared_ptr to the Renderable, may be nullptr if none is set.
             */
            [[nodiscard]] std::shared_ptr<helios::rendering::Renderable> renderable() noexcept;

            /**
             * @brief Applies a scaling transformation to this node's **local** transform.
             * Marks the node dirty. Implementing APIs should consider updating the
             * worldTransform of this node before rendering.
             *
             * @param scale The helios::math::vec3f representing the node's scaling factors.
             *
             * @return A reference to this SceneNode.
             */
            SceneNode& scale(const helios::math::vec3f& scale) noexcept;

            /**
             * @brief Applies rotation to this node's **local** transform.
             * Marks the node dirty. Implementing APIs should consider updating the
             * worldTransform of this node before rendering.
             *
             * @param rotation The helios::math::mat4f rotation matrix.
             *
             * @return A reference to this SceneNode.
             */
            SceneNode& rotate(const helios::math::mat4f& rotation) noexcept;

            /**
             * @brief Applies translation to this node's **local** transform.
             * Marks the node dirty. Implementing APIs should consider updating the
             * worldTransform of this node before rendering.
             *
             * @param translation The helios::math::vec3f representing the node's
             * translation.
             *
             * @return A reference to this SceneNode.
             */
            SceneNode& translate(const helios::math::vec3f& translation) noexcept;

            /**
             * @brief Returns this SceneNode's localTransform.
             *
             * @return A const reference to this SceneNode's `Transform` object.
             */
            [[nodiscard]] const Transform& localTransform() const noexcept;

            /**
             * @brief Returns a pointer to this node's parent node.
             *
             * @return The pointer to this node's parent, or nullptr if no parent exists,
             * e.g. for the root node.
             */
            [[nodiscard]] SceneNode* parent() const noexcept;

            /**
             * @brief Sets the world transform for this SceneNode.
             * Does nothing if the current SceneNode's world transform is considered
             * equal to the specified world transform.
             *
             * @param wf The worldTransform to use for this SceneNode.
             * @param sceneGraphKey The sceneGraphKey as constructed by the `Scene`. Serves
             * as the passkey so this method is not callable from outside
             *
             * @return true if the world transform was updated, otherwise false
             */
            [[nodiscard]] bool setWorldTransform(
                const helios::math::mat4f& wf, helios::scene::SceneGraphKey sceneGraphKey
                ) noexcept;

            /**
             * @brief Computes and returns the world transform for this SceneNode.
             * Will request the current world transform from the
             * parent node to calculate this SceneNode's world transform.
             * For efficiency, this method should cache the result
             * and only trigger a recompute if this SceneNode is dirty (`needsUpdate_ == true`).
             *
             * @return The current world transform matrix for this scene node.
             */
            const helios::math::mat4f& worldTransform() noexcept;

            /**
             * @brief Returns the current worldTransform matrix of this SceneNode
             * **without triggering any recomputation**.
             * This method is efficient for access when the world transform is
             * known to be up-to-date.
             *
             * @return The current worldTransform matrix of this SceneNode.
             */
            [[nodiscard]] const helios::math::mat4f& cachedWorldTransform() const noexcept;

            /**
             * @brief Checks whether this SceneNode needs to be updated, e.g. because the
             * local transformation changed.
             *
             * @return true if this SceneNode is considered to be dirty, otherwise
             * false.
             */
            [[nodiscard]] bool needsUpdate() const noexcept;

    };

} // namespace helios::scene

