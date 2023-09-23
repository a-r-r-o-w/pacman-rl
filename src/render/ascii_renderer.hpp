#ifndef RENDER_ASCII_RENDERER_H
#define RENDER_ASCII_RENDERER_H
#pragma once

#include <iostream>

#include "pacman/state.hpp"

class AsciiRenderer {
  public:
    void render(const State &state) const {
      std::cout << "Lives: " << state.lives << '\n';
      std::cout << " Step: " << state.step_index << '\n';
      std::cout << "Score: " << state.score << "\n\n";

      for (auto &row: state.grid) {
        for (auto &col: row)
          std::cout << col;
        std::cout << '\n';
      }
    }
};

#endif // RENDER_ASCII_RENDERER_H
