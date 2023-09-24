#ifndef PACMAN_ENTITY_H
#define PACMAN_ENTITY_H
#pragma once

#include <ostream>
#include <string>
#include <cmath>

#include "pacman/constants.hpp"
#include "pacman/utils.hpp"
#include "types.hpp"

struct Location {
  i32 x = -1;
  i32 y = -1;

  std::string to_string() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  friend bool operator==(const Location& lhs, const Location& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  friend std::ostream& operator<<(std::ostream& os, const Location& location) {
    os << location.to_string();
    return os;
  }
};

struct GhostConfig {
  i32 step_index = 0;
  i32 chase_steps;
  i32 scatter_steps;
  i32 freight_steps;
  i32 house_steps;
  MovementDirection initial_direction;
  Location initial_location;
  Location corner;
  GhostMode mode;
};

class Entity {
  public:
    EntityType type;
    Location location;
    MovementDirection direction;
  
  public:
    Entity(EntityType type, Location location, MovementDirection direction):
      type(type), location(location), direction(direction)
    { }
    
    virtual ~Entity() = default;

    void set(const Location &location) {
      this->location = location;
    }

    void set(const MovementDirection &direction) {
      this->direction = direction;
    }

    void set(const Location &location, const MovementDirection &direction) {
      set(location);
      set(direction);
    }
};



class Ghost: public Entity {
  public:
    GhostConfig config;
    bool house_state_updated = false;

  public:
    Ghost(EntityType type, Location location, MovementDirection direction, GhostConfig config):
      Entity(type, location, direction),
      config(config)
    { }

    void step(const Location &location, const MovementDirection &direction) {
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

    void set_corner(const Location &corner) {
      config.corner = corner;
    }

    void set_mode(const GhostMode &mode) {
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
};

class Pacman: public Entity {
  public:
    Pacman(Location location, MovementDirection direction):
      Entity(EntityType::pacman, location, direction)
    { }

    ~Pacman() = default;
};

class Blinky: public Ghost {
  public:
    Blinky(GhostConfig config):
      Ghost(EntityType::blinky, config.initial_location, config.initial_direction, config)
    { }

    Location get_target(Pacman* pacman) {
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
};



class Pinky: public Ghost {
  public:
    i32 target_offset;
  
  public:
    Pinky(GhostConfig config, i32 target_offset):
      Ghost(EntityType::pinky, config.initial_location, config.initial_direction, config),
      target_offset(target_offset)
    { }

    Location get_target(Pacman* pacman) {
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
};



class Inky: public Ghost {
  public:
    Inky(GhostConfig config):
      Ghost(EntityType::inky, config.initial_location, config.initial_direction, config)
    { }

    Location get_target(Pacman* pacman, Blinky* blinky) {
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
};



class Clyde: public Ghost {
  public:
    i32 target_switch_distance;
  
  public:
    Clyde(GhostConfig config, i32 target_switch_distance):
      Ghost(EntityType::clyde, config.initial_location, config.initial_direction, config),
      target_switch_distance(target_switch_distance)
    { };

    Location get_target(Pacman* pacman) {
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
};



class Item: public Entity {
  public:
    i32 points;
  
  public:
    Item(EntityType type, Location location, i32 points):
      Entity(type, location, MovementDirection::none),
      points(points)
    { }
    
    ~Item() = default;
};



class Wall: public Entity {
  public:
    Wall(Location location):
      Entity(EntityType::wall, location, MovementDirection::none)
    { }

    ~Wall() = default;
};



class Gate: public Entity {
  public:
    Gate(Location location):
      Entity(EntityType::gate, location, MovementDirection::none)
    { }

    ~Gate() = default;
};

#endif // PACMAN_ENTITY_H
