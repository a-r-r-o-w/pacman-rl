#ifndef PACMAN_STATE_H
#define PACMAN_STATE_H
#pragma once

#include <vector>

#include "types.hpp"

struct State {
  bool completed;
  i32 step_index;
  i32 score;
  std::vector <std::string> grid;
};

#endif // PACMAN_STATE_H
