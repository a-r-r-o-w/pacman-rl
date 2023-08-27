#ifndef HEADER_ENVIRONMENT_H
#define HEADER_ENVIRONMENT_H
#pragma once

#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "pacman/constants.hpp"
#include "pacman/entity.hpp"
#include "pacman/grid.hpp"
#include "pacman/utils.hpp"
#include "types.hpp"

inline GhostConfig default_blinky_config {
  .chase_steps = 30,
  .scatter_steps = 10,
  .freight_steps = 12,
  .house_steps = 0,
  .initial_direction = default_movement_direction[EntityType::blinky],
  .initial_location = {}, // depends on location in grid
  .corner = {}, // depends on grid size
  .mode = default_ghost_mode[EntityType::blinky],
};

inline GhostConfig default_pinky_config {
  .chase_steps = 30,
  .scatter_steps = 10,
  .freight_steps = 12,
  .house_steps = 20,
  .initial_direction = default_movement_direction[EntityType::pinky],
  .initial_location = {}, // depends on location in grid
  .corner = {}, // depends on grid size
  .mode = default_ghost_mode[EntityType::pinky],
};

inline GhostConfig default_inky_config {
  .chase_steps = 30,
  .scatter_steps = 10,
  .freight_steps = 12,
  .house_steps = 40,
  .initial_direction = default_movement_direction[EntityType::inky],
  .initial_location = {}, // depends on location in grid
  .corner = {}, // depends on grid size
  .mode = default_ghost_mode[EntityType::inky],
};

inline GhostConfig default_clyde_config {
  .chase_steps = 30,
  .scatter_steps = 10,
  .freight_steps = 12,
  .house_steps = 60,
  .initial_direction = default_movement_direction[EntityType::clyde],
  .initial_location = {}, // depends on location in grid
  .corner = {}, // depends on grid size
  .mode = default_ghost_mode[EntityType::clyde],
};

struct Config {
  i32 rows;
  i32 cols;
  i32 max_episode_steps;
  std::vector <std::string> map;
  
  GhostConfig blinky_config = default_blinky_config;
  GhostConfig pinky_config  = default_pinky_config;
  GhostConfig inky_config   = default_inky_config;
  GhostConfig clyde_config  = default_clyde_config;
  
  // Ghost specific attributes
  i32 pinky_target_offset = 4;
  i32 clyde_target_switch_distance = 8;
};

struct State {
  bool completed;
  i32 step_index;
};

class Environment {
  private:
    std::vector <Entity*> dynamic_storage;
  
  public:
    Config config;
    State state;
    Grid grid;
    Pacman *pacman;
    Blinky *blinky;
    Pinky *pinky;
    Inky *inky;
    Clyde *clyde;

  public:
    Environment(const Config &c);
    ~Environment();

    void render(const RenderMode &mode = RenderMode::none);
    State step(const MovementDirection &direction);
  
  private:
    std::pair <Location, MovementDirection> perform_pacman_step(const MovementDirection &direction);
    std::pair <Location, MovementDirection> perform_ghost_step(Ghost* ghost, const Location &target);

    bool is_valid_move(const Location &location);;

    void initialize_grid();
    void sync_ghost_config();
    void unset_grid(Entity *entity);
    void set_grid(Entity *entity);
};

#endif // HEADER_ENVIRONMENT_H
