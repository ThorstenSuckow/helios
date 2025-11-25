/**
 * @file _module.ixx
 * @brief Module exports for scene-graph-related functionality.
 */

/**
 * @brief Scene graph and camera management.
 *
 * @details This module provides scene graph functionality including cameras,
 * scene nodes, transformations, culling strategies, and snapshots for managing
 * 3D scene hierarchies and rendering optimization.
 */
export module helios.scene;

export import helios.scene.Camera;
export import helios.scene.CameraSceneNode;
export import helios.scene.CullNoneStrategy;
export import helios.scene.FrustumCullingStrategy;
export import helios.scene.Scene;
export import helios.scene.SceneNode;
export import helios.scene.Snapshot;
export import helios.scene.SnapshotItem;
export import helios.scene.Transform;