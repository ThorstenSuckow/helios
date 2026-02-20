module;

#include <memory>

export module helios.examples.scoring.SpawnConfiguration;

import helios;
import helios.examples.scoring.ArenaConfig;
import helios.examples.scoring.IdConfig;

export namespace helios::examples::scoring {

    using namespace helios::engine::builder::spawnSystem;
    using namespace helios::engine::runtime::pooling;
    using namespace helios::engine::runtime::spawn;
    using namespace helios::engine::core::data;


    constexpr size_t OBJECT_AMOUNT_X = ArenaConfig::GRID_X * ArenaConfig::CELL_LENGTH / ArenaConfig::SPACESHIP_LENGTH;
    constexpr size_t OBJECT_AMOUNT_Y = ArenaConfig::GRID_Y * ArenaConfig::CELL_LENGTH / ArenaConfig::SPACESHIP_LENGTH;

    class SpawnConfiguration {


    public:
        static void make(GameObjectPoolManager& poolManager, SpawnManager& spawnManager) {
        SpawnSystemFactory::configure(poolManager, spawnManager)
            // Projectile pool: emitter-relative placement, no scheduling
            .pool(IdConfig::ProjectilePoolId, IdConfig::ProjectilePrefab, 50)
                .profile(IdConfig::ProjectileSpawnSpawnProfileId)
                    .emitterPlacement()
                    .done()
                .commit()

            // Purple enemy pool: random placement, scheduled every 5s
            .pool(IdConfig::PurpleEnemyPoolId, IdConfig::PurpleEnemyPrefab, 50)
                .profile(IdConfig::RandomSpawnSpawnProfileId)
                    .randomPlacement()
                    .randomDirectionInitializer()
                    .scheduledBy(IdConfig::PurpleEnemySpawnConditionId)
                        .timerCondition(5.0f)
                        .fixedAmount(1)
                        .done()
                    .done()
                .commit()

            // Blue enemy pool: distributed mass spawn, scheduled every 30s
            .pool(IdConfig::BlueEnemyPoolId, IdConfig::BlueEnemyPrefab, 100)
                .profile(IdConfig::BlueMassSpawnProfileId)
                    .placer(std::make_unique<helios::engine::runtime::spawn::behavior::placements::DistributedSpawnPlacer<4>>(
                        ArenaConfig::TOP_LEFT_COORDINATES + helios::math::vec3f(ArenaConfig::CELL_LENGTH, -ArenaConfig::CELL_LENGTH, 0.0f),
                        ArenaConfig::TOP_RIGHT_COORDINATES + helios::math::vec3f(-ArenaConfig::CELL_LENGTH, -ArenaConfig::CELL_LENGTH, 0.0f),
                        ArenaConfig::BOTTOM_LEFT_COORDINATES + helios::math::vec3f(ArenaConfig::CELL_LENGTH, ArenaConfig::CELL_LENGTH, 0.0f),
                        ArenaConfig::BOTTOM_RIGHT_COORDINATES + helios::math::vec3f(-ArenaConfig::CELL_LENGTH, ArenaConfig::CELL_LENGTH, 0.0f)
                    ))
                    .initializer(std::make_unique<helios::engine::runtime::spawn::behavior::initializers::InitializerList<2>>(
                        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::DelayedComponentEnablerInitializer<
                            helios::engine::modules::physics::motion::components::Move2DComponent
                        >>(0.5f, 25),
                        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
                            helios::math::vec3f{0.0f}, helios::engine::runtime::spawn::behavior::initializers::DirectionType::Point
                        )
                    ))
                    .scheduledBy(IdConfig::BlueMassSpawnConditionId)
                        .timerWithAvailabilityCondition(30.0f)
                        .fixedAmount(100)
                        .done()
                    .done()
                .commit()

            // Orange enemy pool: cyclic edge spawning from four directions
            .pool(IdConfig::OrangeEnemyPoolId, IdConfig::OrangeEnemyPrefab, OBJECT_AMOUNT_X)
                .profile(IdConfig::LeftColumnSpawnProfileId)
                    .axisPlacement(helios::math::vec3f(0.0f, -1.0f, 0.0f).normalize(), ArenaConfig::TOP_LEFT_COORDINATES)
                    .moveInitializer(helios::math::X_AXISf)
                    .scheduledBy(IdConfig::OrangeEnemySpawnConditionIdLeftColumn)
                        .timerWithAvailabilityCondition(15.0f)
                        .fixedAmount(OBJECT_AMOUNT_Y)
                        .done()
                    .done()
                .profile(IdConfig::TopRowSpawnProfileId)
                    .axisPlacement(helios::math::vec3f(1.0f, 0.0f, 0.0f).normalize(), ArenaConfig::TOP_LEFT_COORDINATES)
                    .moveInitializer(helios::math::Y_AXISf * -1.0f)
                    .scheduledBy(IdConfig::OrangeEnemySpawnConditionIdTopRow)
                        .timerWithAvailabilityCondition(15.0f)
                        .fixedAmount(OBJECT_AMOUNT_X)
                        .done()
                    .done()
                .profile(IdConfig::RightColumnSpawnProfileId)
                    .axisPlacement(helios::math::vec3f(0.0f, -1.0f, 0.0f).normalize(), ArenaConfig::TOP_RIGHT_COORDINATES)
                    .moveInitializer(helios::math::X_AXISf * -1.0f)
                    .scheduledBy(IdConfig::OrangeEnemySpawnConditionIdRightColumn)
                        .timerWithAvailabilityCondition(15.0f)
                        .fixedAmount(OBJECT_AMOUNT_Y)
                        .done()
                    .done()
                .profile(IdConfig::BottomRowSpawnProfileId)
                    .axisPlacement(helios::math::vec3f(1.0f, 0.0f, 0.0f).normalize(), ArenaConfig::BOTTOM_LEFT_COORDINATES)
                    .moveInitializer(helios::math::Y_AXISf)
                    .scheduledBy(IdConfig::OrangeEnemySpawnConditionIdBottomRow)
                        .timerWithAvailabilityCondition(15.0f)
                        .fixedAmount(OBJECT_AMOUNT_X)
                        .done()
                    .done()
                .commitCyclic<4>();
            }

        };

}