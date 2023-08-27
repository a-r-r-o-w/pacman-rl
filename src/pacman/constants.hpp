#ifndef PACMAN_CONSTANTS_H
#define PACMAN_CONSTANTS_H
#pragma once

#include <utility>

#include "types.hpp"

enum EntityType {
  blinky,
  pinky,
  inky,
  clyde,
  pacman,
  wall,
  gate,
};

enum MovementDirection {
  up,
  left,
  down,
  right,
  none,
};

enum class RenderMode {
  none,
  stdout,
};

enum GhostMode {
  chase,
  scatter,
  freight,
  eaten,
  house,
};

inline constexpr char entity_type_to_char[] = {
  [EntityType::blinky] = '0',
  [EntityType::pinky]  = '1',
  [EntityType::inky]   = '2',
  [EntityType::clyde]  = '3',
  [EntityType::pacman] = 'P',
  [EntityType::wall]   = '#',
  [EntityType::gate]   = 'G',
};

inline constexpr i32 movement_direction_delta_x[] = {
  [MovementDirection::up]    = -1,
  [MovementDirection::left]  = 0,
  [MovementDirection::down]  = +1,
  [MovementDirection::right] = 0,
  [MovementDirection::none]  = 0,
};

inline constexpr i32 movement_direction_delta_y[] = {
  [MovementDirection::up]    = 0,
  [MovementDirection::left]  = -1,
  [MovementDirection::down]  = 0,
  [MovementDirection::right] = +1,
  [MovementDirection::none]  = 0,
};

inline constexpr MovementDirection default_movement_direction[] = {
  [EntityType::blinky] = MovementDirection::left,
  [EntityType::pinky]  = MovementDirection::none,
  [EntityType::inky]   = MovementDirection::none,
  [EntityType::clyde]  = MovementDirection::none,
  [EntityType::pacman] = MovementDirection::none,
};

inline constexpr GhostMode default_ghost_mode[] = {
  [EntityType::blinky] = GhostMode::chase,
  [EntityType::pinky]  = GhostMode::house,
  [EntityType::inky]   = GhostMode::house,
  [EntityType::clyde]  = GhostMode::house,
};

inline constexpr MovementDirection opposite_direction[] = {
  [MovementDirection::up]    = MovementDirection::down,
  [MovementDirection::left]  = MovementDirection::right,
  [MovementDirection::down]  = MovementDirection::up,
  [MovementDirection::right] = MovementDirection::left,
  [MovementDirection::none]  = MovementDirection::none,
};

inline constexpr MovementDirection movement_direction_precedence[] = {
  MovementDirection::up, MovementDirection::left, MovementDirection::down, MovementDirection::right
};

inline constexpr MovementDirection legal_ghost_movement_direction[][3] = {
  [MovementDirection::up]    = { MovementDirection::up, MovementDirection::left, MovementDirection::right },
  [MovementDirection::left]  = { MovementDirection::left, MovementDirection::up, MovementDirection::down },
  [MovementDirection::down]  = { MovementDirection::down, MovementDirection::left, MovementDirection::right },
  [MovementDirection::right] = { MovementDirection::right, MovementDirection::down, MovementDirection::up },
  [MovementDirection::none]  = { MovementDirection::none, MovementDirection::none, MovementDirection::none },
};

inline constexpr const char* direction_to_string[] = {
  [MovementDirection::up]    = "up",
  [MovementDirection::left]  = "left",
  [MovementDirection::down]  = "down",
  [MovementDirection::right] = "right",
  [MovementDirection::none]  = "none",
};

inline constexpr const char* ghost_mode_to_string[] = {
  [GhostMode::chase]   = "chase",
  [GhostMode::scatter] = "scatter",
  [GhostMode::freight] = "freight",
  [GhostMode::eaten]   = "eaten",
  [GhostMode::house]   = "house",
};

inline i32 i32_inf = i32(1) << 30;
inline i64 i64_inf = i64(1) << 30;

#endif // PACMAN_CONSTANTS_H
