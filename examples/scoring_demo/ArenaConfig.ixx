module;



export module helios.examples.scoring.ArenaConfig;

import helios.math;

export namespace helios::examples::scoring::ArenaConfig{

    constexpr float CELL_LENGTH        = 5.0f;
    constexpr float SPACESHIP_LENGTH   = 5.0f;
    constexpr float GRID_X             = 29.0f;
    constexpr float GRID_Y             = 19.0f;

    constexpr helios::math::vec3f GRID_SCALE{GRID_X * CELL_LENGTH, GRID_Y * CELL_LENGTH, 0.0f};

    constexpr auto TOP_LEFT_COORDINATES = helios::math::vec3f(GRID_SCALE[0] * - 0.5f, GRID_SCALE[1] * 0.5f, 0.0f);
    constexpr auto TOP_RIGHT_COORDINATES = helios::math::vec3f(GRID_SCALE[0] *  0.5f, GRID_SCALE[1] * 0.5f, 0.0f);
    constexpr auto BOTTOM_LEFT_COORDINATES =  helios::math::vec3f(GRID_SCALE[0] * - 0.5f, GRID_SCALE[1] * -0.5f, 0.0f);
    constexpr auto BOTTOM_RIGHT_COORDINATES =  helios::math::vec3f(GRID_SCALE[0] * 0.5f, GRID_SCALE[1] * -0.5f, 0.0f);

    constexpr auto BLUE_ENEMY_SCALE = helios::math::vec3f{SPACESHIP_LENGTH, SPACESHIP_LENGTH/2.0f, 0.0f};

    constexpr auto SPACESHIP_SPAWN_POSITION = helios::math::vec3f(0.0f, 0.0f, 0.0f);
    constexpr auto SPACESHIP_SIZE  = helios::math::vec3f(SPACESHIP_LENGTH, SPACESHIP_LENGTH, 0.0f);


}