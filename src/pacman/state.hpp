#ifndef PACMAN_STATE_H
#define PACMAN_STATE_H
#include "entity.hpp"
#pragma once

#include <string>
#include <vector>

#include "types.hpp"

struct State {
  i32 step_index;
  i32 score;
  i32 lives;
  
  bool completed;

  Location pacman_location;
  Location blinky_location;
  Location pinky_location;
  Location inky_location;
  Location clyde_location;
  
  std::vector <std::string> grid;
};

#endif // PACMAN_STATE_H
