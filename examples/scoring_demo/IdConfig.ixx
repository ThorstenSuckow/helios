/**
 * @file IdConfig.ixx
 * @brief Centralized ID constants for the scoring demo.
 */
module;



export module helios.examples.scoring.IdConfig;

import helios.math;
import helios.engine.core.data;

export namespace helios::examples::scoring::IdConfig {

   using namespace helios::engine::core::data;

    constexpr PrefabId BlueEnemyPrefab("BlueEnemy");
    constexpr PrefabId OrangeEnemyPrefab("OrangeEnemy");
    constexpr PrefabId PurpleEnemyPrefab("PurpleEnemy");
    constexpr PrefabId ProjectilePrefab("Projectile");


    constexpr SpawnProfileId ProjectileSpawnSpawnProfileId{"projectile_spawn"};
    constexpr SpawnProfileId RandomSpawnSpawnProfileId{"random_spawn"};
    constexpr SpawnProfileId BlueMassSpawnProfileId{"blue_spawn_profile1"};
    constexpr SpawnProfileId LeftColumnSpawnProfileId{"orange_left_spawn1"};
    constexpr SpawnProfileId RightColumnSpawnProfileId{"orange_right_spawn_1"};
    constexpr SpawnProfileId TopRowSpawnProfileId{"orange_top_spawn_1"};
    constexpr SpawnProfileId BottomRowSpawnProfileId{"orange_bottom_spawn_1"};

    constexpr SpawnRuleId PurpleEnemySpawnConditionId{"purple_spawn_rule"};
    constexpr SpawnRuleId BlueMassSpawnConditionId{"blue_mass_spawn1"};
    constexpr SpawnRuleId OrangeEnemySpawnConditionIdLeftColumn{"orange_spawn_rule_1"};
    constexpr SpawnRuleId OrangeEnemySpawnConditionIdRightColumn{"orange_spawn_rule_2"};
    constexpr SpawnRuleId OrangeEnemySpawnConditionIdTopRow{"orange_spawn_rule_3"};
    constexpr SpawnRuleId OrangeEnemySpawnConditionIdBottomRow{"orange_spawn_rule_4"};

    constexpr GameObjectPoolId PurpleEnemyPoolId{"purple_pool"};
    constexpr GameObjectPoolId OrangeEnemyPoolId{"orange_pool"};
    constexpr GameObjectPoolId BlueEnemyPoolId{"blue_pool"};
    constexpr GameObjectPoolId ProjectilePoolId{"projectile_pool"};
}