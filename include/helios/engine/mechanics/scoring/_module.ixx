
/**
 * @file _module.ixx
 * @brief Module facade for the scoring system.
 */
export module helios.engine.mechanics.scoring;

export import helios.engine.mechanics.scoring.commands;
export import helios.engine.mechanics.scoring.components;
export import helios.engine.mechanics.scoring.systems;
export import helios.engine.mechanics.scoring.types;

export import helios.engine.mechanics.scoring.ScorePoolManager;
export import helios.engine.mechanics.scoring.ScorePool;
export import helios.engine.mechanics.scoring.ScorePoolSnapshot;
export import helios.engine.mechanics.scoring.MaxScorePoolSnapshot;
export import helios.engine.mechanics.scoring.ScoreCommandHandler;
export import helios.engine.mechanics.scoring.ScoreCommandDispatcher;