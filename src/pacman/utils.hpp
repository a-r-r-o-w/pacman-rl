#ifndef PACMAN_UTILS_HPP
#define PACMAN_UTILS_HPP
#pragma once

#include <cmath>

#include "types.hpp"

inline i32 manhattan_distance(i32 x1, i32 y1, i32 x2, i32 y2) {
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

#endif // PACMAN_UTILS_HPP
