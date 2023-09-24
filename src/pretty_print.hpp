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
    "  .initial_direction = " + direction_to_string[ghost_config.initial_direction] + ",\n"
    "  .initial_location = " + ghost_config.initial_location.to_string() + ",\n"
    "  .corner = " + ghost_config.corner.to_string() + ",\n"
    "  .mode = " + ghost_mode_to_string[ghost_config.mode] + "\n"
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

#endif // BINDINGS_PRETTY_PRINT_H
