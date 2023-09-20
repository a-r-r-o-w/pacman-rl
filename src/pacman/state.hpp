#ifndef PACMAN_STATE_H
#define PACMAN_STATE_H
#pragma once

#include <vector>

#include "types.hpp"

struct State {
  i32 step_index;
  i32 score;
  i32 lives;
  bool completed;
  std::vector <std::string> grid;
};

#endif // PACMAN_STATE_H
