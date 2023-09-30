#ifndef PACMAN_CONSTANTS_H
#define PACMAN_CONSTANTS_H
#include <unordered_map>
#pragma once

#include <ostream>
#include <utility>

#include "types.hpp"

enum class EntityType {
  blinky,
  pinky,
  inky,
  clyde,
  pacman,
  wall,
  gate,
  pellet,
  power_pellet,
  none,
};

enum class MovementDirection {
  up,
  left,
  down,
  right,
  none,
};

enum class GhostMode {
  chase,
  scatter,
  freight,
  eaten,
  house,
};

enum class RenderMode {
  ascii,
  human,
  none,
};

inline char entity_type_to_char(EntityType entity_type) {
  static const std::unordered_map<EntityType, char> entity_type_to_char = {
    { EntityType::blinky,       '0' },
    { EntityType::pinky,        '1' },
    { EntityType::inky,         '2' },
    { EntityType::clyde,        '3' },
    { EntityType::pacman,       'P' },
    { EntityType::wall,         '#' },
    { EntityType::gate,         'G' },
    { EntityType::pellet,       '.' },
    { EntityType::power_pellet, '@' },
    { EntityType::none,         ' ' },
  };

  return entity_type_to_char.at(entity_type);
}

inline EntityType char_to_entity_type(char c) {
  static const std::unordered_map<char, EntityType> char_to_entity_type = {
    { '0', EntityType::blinky },
    { '1', EntityType::pinky },
    { '2', EntityType::inky },
    { '3', EntityType::clyde },
    { 'P', EntityType::pacman },
    { '#', EntityType::wall },
    { 'G', EntityType::gate },
    { '.', EntityType::pellet },
    { '@', EntityType::power_pellet },
    { ' ', EntityType::none },
  };

  return char_to_entity_type.at(c);
}

inline i32 entity_type_render_precedence(EntityType entity_type) {
  static const std::unordered_map<EntityType, i32> entity_type_render_precedence = {
    { EntityType::blinky,       1 },
    { EntityType::pinky,        1 },
    { EntityType::inky,         1 },
    { EntityType::clyde,        1 },
    { EntityType::pacman,       2 },
    { EntityType::wall,         3 },
    { EntityType::gate,         3 },
    { EntityType::pellet,       0 },
    { EntityType::power_pellet, 0 },
    { EntityType::none,         9 },
  };

  return entity_type_render_precedence.at(entity_type);
}

inline i32 movement_direction_delta_x(MovementDirection direction) {
  static const std::unordered_map<MovementDirection, i32> movement_direction_delta_x = {
    { MovementDirection::up,    -1 },
    { MovementDirection::left,   0 },
    { MovementDirection::down,  +1 },
    { MovementDirection::right,  0 },
    { MovementDirection::none,   0 },
  };

  return movement_direction_delta_x.at(direction);
}

inline i32 movement_direction_delta_y(MovementDirection direction) {
  static const std::unordered_map<MovementDirection, i32> movement_direction_delta_y = {
    { MovementDirection::up,     0 },
    { MovementDirection::left,  -1 },
    { MovementDirection::down,   0 },
    { MovementDirection::right, +1 },
    { MovementDirection::none,   0 },
  };

  return movement_direction_delta_y.at(direction);
}

inline MovementDirection default_movement_direction(EntityType entity_type) {
  static const std::unordered_map<EntityType, MovementDirection> default_movement_direction = {
    { EntityType::blinky,       MovementDirection::left },
    { EntityType::pinky,        MovementDirection::none },
    { EntityType::inky,         MovementDirection::none },
    { EntityType::clyde,        MovementDirection::none },
    { EntityType::pacman,       MovementDirection::none },
  };

  auto it = default_movement_direction.find(entity_type);
  if (it == default_movement_direction.end())
    throw std::runtime_error("Invalid entity type");
  return it->second;
}

inline GhostMode default_ghost_mode(EntityType entity_type) {
  static const std::unordered_map<EntityType, GhostMode> default_ghost_mode = {
    { EntityType::blinky,       GhostMode::chase },
    { EntityType::pinky,        GhostMode::house },
    { EntityType::inky,         GhostMode::house },
    { EntityType::clyde,        GhostMode::house },
  };

  auto it = default_ghost_mode.find(entity_type);
  if (it == default_ghost_mode.end())
    throw std::runtime_error("Invalid entity type");
  return it->second;
}

inline constexpr MovementDirection opposite_direction(MovementDirection direction) {
  switch (direction) {
    case MovementDirection::up:    return MovementDirection::down;
    case MovementDirection::left:  return MovementDirection::right;
    case MovementDirection::down:  return MovementDirection::up;
    case MovementDirection::right: return MovementDirection::left;
    case MovementDirection::none:  return MovementDirection::none;
  }
  throw std::runtime_error("Invalid direction");
}

inline constexpr MovementDirection movement_direction_precedence[] = {
  MovementDirection::up, MovementDirection::left, MovementDirection::down, MovementDirection::right
};

inline constexpr const char* movement_direction_to_string(MovementDirection direction) {
  switch (direction) {
    case MovementDirection::up:    return "up";
    case MovementDirection::left:  return "left";
    case MovementDirection::down:  return "down";
    case MovementDirection::right: return "right";
    case MovementDirection::none:  return "none";
  }
  __builtin_unreachable();
}

inline constexpr const char* ghost_mode_to_string(GhostMode mode) {
  switch (mode) {
    case GhostMode::chase:   return "chase";
    case GhostMode::scatter: return "scatter";
    case GhostMode::freight: return "freight";
    case GhostMode::eaten:   return "eaten";
    case GhostMode::house:   return "house";
  }
  __builtin_unreachable();
}

inline i32 i32_inf = i32(1) << 30;
inline i64 i64_inf = i64(1) << 30;

inline std::ostream& operator<<(std::ostream& os, const EntityType& entity_type) {
  os << entity_type_to_char(entity_type);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const MovementDirection& direction) {
  os << movement_direction_to_string(direction);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const GhostMode& mode) {
  os << ghost_mode_to_string(mode);
  return os;
}

#endif // PACMAN_CONSTANTS_H
