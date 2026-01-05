/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::game::components namespace.
 */
export module helios.engine.game.components;

// Input components
export import helios.engine.game.components.input.TwinStickInputComponent;

// Collision components
export import helios.engine.game.physics.collision.components.AabbColliderComponent;
export import helios.engine.game.physics.collision.components.CollisionComponent;

// Motion components
export import helios.engine.game.spatial.transform.components.TransformComponent;
export import helios.engine.game.physics.motion.components.HeadingComponent;
export import helios.engine.game.physics.motion.components.Move2DComponent;
export import helios.engine.game.physics.motion.components.RotationStateComponent;
export import helios.engine.game.physics.motion.components.SpinComponent;

// Spatial components
export import helios.engine.game.spatial.transform.components.ScaleComponent;
export import helios.engine.game.spatial.transform.components.TransformComponent;
export import helios.engine.game.spatial.transform.components.TranslationStateComponent;

// Gameplay bounds components
export import helios.engine.game.gameplay.bounds.components.LevelBoundsBehaviorComponent;

// Scene components
export import helios.engine.game.components.scene.SceneNodeComponent;

// Rendering components
export import helios.engine.game.components.rendering.RenderableComponent;

// Model components
export import helios.engine.game.components.model.ModelAabbComponent;



