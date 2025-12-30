/**
 * @file _module.ixx
 * @brief Aggregate module for helios::engine::game::components namespace.
 */
export module helios.engine.game.components;

// Input components
export import helios.engine.game.components.input.TwinStickInputComponent;

// Physics components
export import helios.engine.game.components.physics.AabbColliderComponent;
export import helios.engine.game.components.physics.DirectionComponent;
export import helios.engine.game.components.physics.HeadingComponent;
export import helios.engine.game.components.physics.LevelBoundsBehaviorComponent;
export import helios.engine.game.components.physics.Move2DComponent;
export import helios.engine.game.components.physics.RotationStateComponent;
export import helios.engine.game.components.physics.ScaleComponent;
export import helios.engine.game.components.physics.SpinComponent;
export import helios.engine.game.components.physics.TransformComponent;
export import helios.engine.game.components.physics.TranslationStateComponent;

// Scene components
export import helios.engine.game.components.scene.SceneNodeComponent;

// Rendering components
export import helios.engine.game.components.rendering.RenderableComponent;

// Model components
export import helios.engine.game.components.model.ModelAabbComponent;

// Gameplay components
export import helios.engine.game.components.gameplay.Aim2DComponent;
export import helios.engine.game.components.gameplay.ShootComponent;

