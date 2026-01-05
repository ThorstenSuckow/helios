/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::game::components namespace.
 */
export module helios.engine.game.components;

// Input components
export import helios.engine.game.components.input.TwinStickInputComponent;

// Physics components
export import helios.engine.game.physics.components.AabbColliderComponent;
export import helios.engine.game.physics.components.DirectionComponent;
export import helios.engine.game.physics.components.HeadingComponent;
export import helios.engine.game.physics.components.LevelBoundsBehaviorComponent;
export import helios.engine.game.physics.components.Move2DComponent;
export import helios.engine.game.physics.components.RotationStateComponent;
export import helios.engine.game.physics.components.ScaleComponent;
export import helios.engine.game.physics.components.SpinComponent;
export import helios.engine.game.physics.components.TransformComponent;
export import helios.engine.game.physics.components.TranslationStateComponent;

// Scene components
export import helios.engine.game.components.scene.SceneNodeComponent;

// Rendering components
export import helios.engine.game.components.rendering.RenderableComponent;

// Model components
export import helios.engine.game.components.model.ModelAabbComponent;

// Gameplay components
export import helios.engine.game.gameplay.components.Aim2DComponent;
export import helios.engine.game.gameplay.components.ShootComponent;

