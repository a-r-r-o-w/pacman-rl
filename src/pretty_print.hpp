#ifndef BINDINGS_PRETTY_PRINT_H
#define BINDINGS_PRETTY_PRINT_H

#include <string>
#include <vector>

#include "pacman/constants.hpp"
#include "pacman/entity.hpp"
#include "environment.hpp"

inline std::string pretty_location(const Location &location) {
  return
    "pacman_rl.Location{\n"
    "  .x = " + std::to_string(location.x) + ",\n"
    "  .y = " + std::to_string(location.y) + "\n"
    "}";
}

inline std::string pretty_ghost_config(const GhostConfig &ghost_config) {
  return
    "pacman_rl.GhostConfig{\n"
    "  .step_index = " + std::to_string(ghost_config.step_index) + ",\n"
    "  .chase_steps = " + std::to_string(ghost_config.chase_steps) + ",\n"
    "  .scatter_steps = " + std::to_string(ghost_config.scatter_steps) + ",\n"
    "  .freight_steps = " + std::to_string(ghost_config.freight_steps) + ",\n"
    "  .house_steps = " + std::to_string(ghost_config.house_steps) + ",\n"
    "  .initial_direction = " + movement_direction_to_string(ghost_config.initial_direction) + ",\n"
    "  .initial_location = " + ghost_config.initial_location.to_string() + ",\n"
    "  .corner = " + ghost_config.corner.to_string() + ",\n"
    "  .mode = " + ghost_mode_to_string(ghost_config.mode) + "\n"
    "}";
}

inline std::string pretty_map(const std::vector<std::string> &map) {
  std::string result = "std::vector<std::string>{\n";
  for (const std::string &row: map) {
    result += "  \"" + row + "\",\n";
  }
  result += "}";
  return result;
}

inline std::string pretty_config(const Config &config) {
  return
    "pacman_rl.Config{\n"
    "  .rows = " + std::to_string(config.rows) + ",\n"
    "  .cols = " + std::to_string(config.cols) + ",\n"
    "  .max_episode_steps = " + std::to_string(config.max_episode_steps) + ",\n"
    "  .map = " + pretty_map(config.map) + ",\n"
    "  .blinky_config = " + pretty_ghost_config(config.blinky_config) + ",\n"
    "  .pinky_config = " + pretty_ghost_config(config.pinky_config) + ",\n"
    "  .inky_config = " + pretty_ghost_config(config.inky_config) + ",\n"
    "  .clyde_config = " + pretty_ghost_config(config.clyde_config) + ",\n"
    "  .pacman_lives = " + std::to_string(config.pacman_lives) + ",\n"
    "  .score_per_ghost_eaten = " + std::to_string(config.score_per_ghost_eaten) + ",\n"
    "  .pinky_target_offset = " + std::to_string(config.pinky_target_offset) + ",\n"
    "  .clyde_target_switch_distance = " + std::to_string(config.clyde_target_switch_distance) + ",\n"
    "  .pellet_points = " + std::to_string(config.pellet_points) + ",\n"
    "  .power_pellet_points = " + std::to_string(config.power_pellet_points) + ",\n"
    "  .power_pellet_steps = " + std::to_string(config.power_pellet_steps) + "\n"
    "}";
}

inline std::string pretty_state(const State &state) {
  std::string result =
    "pacman_rl.State{\n"
    "  .step_index = " + std::to_string(state.step_index) + ",\n"
    "  .score = " + std::to_string(state.score) + ",\n"
    "  .lives = " + std::to_string(state.lives) + ",\n"
    "  .completed = " + std::string(state.completed ? "true" : "false") + ",\n"
    "  .pacman_location = " + pretty_location(state.pacman_location) + ",\n"
    "  .blinky_location = " + pretty_location(state.blinky_location) + ",\n"
    "  .pinky_location = " + pretty_location(state.pinky_location) + ",\n"
    "  .inky_location = " + pretty_location(state.inky_location) + ",\n"
    "  .clyde_location = " + pretty_location(state.clyde_location) + ",\n"
    "  .grid = std::vector<std::string>{\n";
  for (const std::string &row: state.grid)
    result += "    \"" + row + "\",\n";
  result +=
    "  }\n"
    "}";
  return result;
}

// class Environment {
//   private:
//     Config config;
//     State state;
//     Grid grid;
    
//     std::unique_ptr<Pacman> pacman;
//     std::unique_ptr<Blinky> blinky;
//     std::unique_ptr<Pinky> pinky;
//     std::unique_ptr<Inky> inky;
//     std::unique_ptr<Clyde> clyde;
//     std::vector<std::unique_ptr<Entity>> entities;
//     std::vector<std::pair<i32, Entity*>> grid_storage;
    
//     AsciiRenderer ascii_renderer;

//     Location initial_pacman_location = {};

inline std::string pretty_environment(const Environment &env) {
  return
    "pacman_rl.Environment{\n"
    "  .config = " + pretty_config(env.config) + ",\n"
    "  .state = " + pretty_state(env.state) + ",\n"
    // "  .pacman = " + env.pacman->to_string() + ",\n"
    // "  .blinky = " + env.blinky->to_string() + ",\n"
    // "  .pinky = " + env.pinky->to_string() + ",\n"
    // "  .inky = " + env.inky->to_string() + ",\n"
    // "  .clyde = " + env.clyde->to_string() + "\n"
    "}";
}

#endif // BINDINGS_PRETTY_PRINT_H
