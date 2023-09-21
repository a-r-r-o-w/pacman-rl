#include <cmath>

#include "pacman/entity.hpp"

void Entity::set(const Location &location) {
  this->location = location;
}

void Entity::set(const MovementDirection &direction) {
  this->direction = direction;
}

void Entity::set(const Location &location, const MovementDirection &direction) {
  this->location = location;
  this->direction = direction;
}



Pacman::Pacman(Location location, MovementDirection direction):
  Entity(EntityType::pacman, location, direction)
{ }



void Ghost::step(const Location &location, const MovementDirection &direction) {
  this->location = location;
  this->direction = direction;
  config.step_index += 1;

  switch (config.mode) {
    case GhostMode::chase: {
      if (config.step_index >= config.chase_steps) {
        config.step_index = 0;
        set_mode(GhostMode::scatter);
      }
    }
    break;
    
    case GhostMode::scatter: {
      if (config.step_index >= config.scatter_steps) {
        config.step_index = 0;
        set_mode(GhostMode::chase);
      }
    }
    break;
    
    case GhostMode::freight: {
      if (config.step_index >= config.freight_steps) {
        config.step_index = 0;
        set_mode(GhostMode::chase);
      }
    }
    break;
    
    case GhostMode::eaten:
      break;
    
    case GhostMode::house: {
      if (config.step_index >= config.house_steps) {
        config.step_index = 0;
        house_state_updated = true;
        set_mode(GhostMode::chase);
      }
    }
    break;
  }
}

void Ghost::set_corner(const Location &corner) {
  config.corner = corner;
}

void Ghost::set_mode(const GhostMode &mode) {
  if (config.mode == mode) {
    if (config.mode == GhostMode::freight)
      config.step_index = 0;
    return;
  }
  
  // Ignore mode change if target is freight but the ghost is inside house
  if (mode == GhostMode::freight and config.mode == GhostMode::house)
    return;
  
  if (config.mode == GhostMode::chase or config.mode == GhostMode::scatter)
    direction = opposite_direction[direction];
  config.mode = mode;
  config.step_index = 0;
}

Blinky::Blinky(GhostConfig config):
  Ghost(EntityType::blinky, config.initial_location, config.initial_direction, config)
{ }

Location Blinky::get_target(Pacman* pacman) {
  switch (config.mode) {
    case GhostMode::chase: {
      return Location{
        .x = pacman->location.x,
        .y = pacman->location.y
      };
    }
    
    case GhostMode::scatter:
      return config.corner;
    
    case GhostMode::freight: return config.corner;
    case GhostMode::eaten: return config.corner;
    
    case GhostMode::house:
      return config.initial_location;

    default:
      __builtin_unreachable();
  }
}

Pinky::Pinky(GhostConfig config, i32 target_offset):
  Ghost(EntityType::pinky, config.initial_location, config.initial_direction, config),
  target_offset(target_offset)
{ }

Location Pinky::get_target(Pacman* pacman) {
  switch (config.mode) {
    case GhostMode::chase: {
      i32 parity_x = movement_direction_delta_x[pacman->direction] < 0 ? -1 : +1;
      i32 parity_y = movement_direction_delta_y[pacman->direction] < 0 ? -1 : +1;
      Location target = {
        .x = pacman->location.x + target_offset * parity_x,
        .y = pacman->location.y + target_offset * parity_y
      };
      return target;
    }

    case GhostMode::scatter:
      return config.corner;
    
    case GhostMode::freight: return config.corner;
    case GhostMode::eaten: return config.corner;
    
    case GhostMode::house:
      return config.initial_location;

    default:
      __builtin_unreachable();
  }
}

Inky::Inky(GhostConfig config):
  Ghost(EntityType::inky, config.initial_location, config.initial_direction, config)
{ }

Location Inky::get_target(Pacman* pacman, Blinky* blinky) {
  switch (config.mode) {
    case GhostMode::chase: {
      i32 blinky_distance_x = std::abs(pacman->location.x - blinky->location.x);
      i32 blinky_distance_y = std::abs(pacman->location.y - blinky->location.y);
      i32 parity_x = blinky->location.x < pacman->location.x ? +1 : -1;
      i32 parity_y = blinky->location.y < pacman->location.y ? +1 : -1;
      Location target = {
        .x = pacman->location.x + blinky_distance_x * parity_x,
        .y = pacman->location.y + blinky_distance_y * parity_y
      };
      return target;
    }

    case GhostMode::scatter:
      return config.corner;
    
    case GhostMode::freight: return config.corner;
    case GhostMode::eaten: return config.corner;

    case GhostMode::house:
      return config.initial_location;

    default:
      __builtin_unreachable();
  }
}

Clyde::Clyde(GhostConfig config, i32 target_switch_distance):
  Ghost(EntityType::clyde, config.initial_location, config.initial_direction, config),
  target_switch_distance(target_switch_distance)
{ }

Location Clyde::get_target(Pacman* pacman) {
  switch (config.mode) {
    case GhostMode::chase: {
      i32 distance = manhattan_distance(location.x, location.y, pacman->location.x, pacman->location.y);
      Location target = {
        .x = pacman->location.x,
        .y = pacman->location.y
      };
      if (distance < target_switch_distance)
        target = config.corner;
      return target;
    }

    case GhostMode::scatter:
      return config.corner;
    
    case GhostMode::freight: return config.corner;
    case GhostMode::eaten: return config.corner;

    case GhostMode::house:
      return config.initial_location;
    
    default:
      __builtin_unreachable();
  }
}



Item::Item(EntityType type, Location location, i32 points):
  Entity(type, location, MovementDirection::none),
  points(points)
{ }



Wall::Wall(Location location):
  Entity(EntityType::wall, location, MovementDirection::none)
{ }



Gate::Gate(Location location):
  Entity(EntityType::gate, location, MovementDirection::none)
{ }
