#include <iostream>

#include "ascii_renderer.hpp"

void AsciiRenderer::render(const State &state) const {
  std::cout << "Lives: " << state.lives << '\n';
  std::cout << " Step: " << state.step_index << '\n';
  std::cout << "Score: " << state.score << "\n\n";

  for (auto &row: state.grid) {
    for (auto &col: row)
      std::cout << col;
    std::cout << '\n';
  }
}
