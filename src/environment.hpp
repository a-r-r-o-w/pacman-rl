#ifndef HEADER_ENVIRONMENT_H
#define HEADER_ENVIRONMENT_H
#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "types.hpp"
#include "pacman/constants.hpp"
#include "pacman/entity.hpp"
#include "pacman/grid.hpp"
#include "pacman/state.hpp"
#include "pacman/utils.hpp"
#include "render/ascii_renderer.hpp"

inline GhostConfig default_blinky_config {
  .chase_steps = 30,
  .scatter_steps = 18,
  .freight_steps = 16,
  .house_steps = 0,
  .initial_direction = default_movement_direction[EntityType::blinky],
  .initial_location = {}, // depends on location in grid
  .corner = {}, // depends on grid size
  .mode = default_ghost_mode[EntityType::blinky],
};

inline GhostConfig default_pinky_config {
  .chase_steps = 30,
  .scatter_steps = 18,
  .freight_steps = 16,
  .house_steps = 20,
  .initial_direction = default_movement_direction[EntityType::pinky],
  .initial_location = {}, // depends on location in grid
  .corner = {}, // depends on grid size
  .mode = default_ghost_mode[EntityType::pinky],
};

inline GhostConfig default_inky_config {
  .chase_steps = 30,
  .scatter_steps = 18,
  .freight_steps = 16,
  .house_steps = 40,
  .initial_direction = default_movement_direction[EntityType::inky],
  .initial_location = {}, // depends on location in grid
  .corner = {}, // depends on grid size
  .mode = default_ghost_mode[EntityType::inky],
};

inline GhostConfig default_clyde_config {
  .chase_steps = 30,
  .scatter_steps = 18,
  .freight_steps = 16,
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

  // Pacman specific attributes
  i32 pacman_lives = 3;
  
  // Ghost specific attributes
  i32 pinky_target_offset = 4;
  i32 clyde_target_switch_distance = 8;

  // Pellet scores
  i32 pellet_points = 10;
  i32 power_pellet_points = 50;

  // Number of steps for which the effect of power pellet lasts
  i32 power_pellet_steps = 20;
};

class Environment {
  private:
    Config config;
    State state;
    Grid grid;
    
    std::unique_ptr <Pacman> pacman;
    std::unique_ptr <Blinky> blinky;
    std::unique_ptr <Pinky> pinky;
    std::unique_ptr <Inky> inky;
    std::unique_ptr <Clyde> clyde;
    std::vector <std::unique_ptr <Entity>> entities;
    std::vector <std::pair <i32, Entity*>> grid_storage;
    
    AsciiRenderer ascii_renderer;

    Location _initial_pacman_location;

    using Step = std::pair <Location, MovementDirection>;
  
  public:  
    Environment(const Config &c);
    ~Environment();

    void render(const RenderMode &mode = RenderMode::none);
    State step(const MovementDirection &direction);
  
  private:
    Step perform_pacman_step(const MovementDirection &direction);
    Step perform_ghost_step(Ghost* ghost, Location target);
    void restore_original_state();
    void handle_pacman_death();

    bool is_valid_pacman_move(const Location &location);
    bool is_valid_ghost_move(Ghost *ghost, const Location &location);

    void initialize_grid();
    void assign_state_grid_from_grid();
    void sync_ghost_config();
    void unset_grid(Entity *entity);
    void set_grid(Entity *entity);
};

#endif // HEADER_ENVIRONMENT_H
