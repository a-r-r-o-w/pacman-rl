#ifndef RENDER_ASCII_RENDERER_H
#define RENDER_ASCII_RENDERER_H
#pragma once

#include "pacman/state.hpp"

class AsciiRenderer {
  public:
    void render(const State &state) const;
};

#endif // RENDER_ASCII_RENDERER_H
