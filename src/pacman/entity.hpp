#ifndef PACMAN_ENTITY_H
#define PACMAN_ENTITY_H
#pragma once

#include "pacman/constants.hpp"
#include "pacman/utils.hpp"
#include "types.hpp"

struct Location {
  i32 x = -1;
  i32 y = -1;
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

    void set(const Location &location);
    void set(const MovementDirection &direction);
    void set(const Location &location, const MovementDirection &direction);
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

    void step(const Location &location, const MovementDirection &direction);

    void set_corner(const Location &corner);
    void set_mode(const GhostMode &mode);
};

class Pacman: public Entity {
  public:
    Pacman(Location location, MovementDirection direction);
    ~Pacman() = default;
};

class Blinky: public Ghost {
  public:
    Blinky(GhostConfig config);

    Location get_target(Pacman* pacman);
};

class Pinky: public Ghost {
  public:
    i32 target_offset;
  
  public:
    Pinky(GhostConfig config, i32 target_offset);

    Location get_target(Pacman* pacman);
};

class Inky: public Ghost {
  public:
    Inky(GhostConfig config);

    Location get_target(Pacman* pacman, Blinky* blinky);
};

class Clyde: public Ghost {
  public:
    i32 target_switch_distance;
  
  public:
    Clyde(GhostConfig config, i32 target_switch_distance);

    Location get_target(Pacman* pacman);
};

class Item: public Entity {
  public:
    i32 points;
  
  public:
    Item(EntityType type, Location location, i32 points);
    ~Item() = default;
};

class Wall: public Entity {
  public:
    Wall(Location location);
    ~Wall() = default;
};

class Gate: public Entity {
  public:
    Gate(Location location);
    ~Gate() = default;
};

#endif // PACMAN_ENTITY_H
