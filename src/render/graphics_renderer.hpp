#ifndef RENDER_GRAPHICS_RENDERER_H
#define RENDER_GRAPHICS_RENDERER_H
#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "raylib.h"

#include "constants.hpp"
#include "types.hpp"
#include "pacman/state.hpp"
#include "render_utils.hpp"

#define LIGHTBLACK (Color{24, 24, 24, 255})

class GraphicsRenderer {
  private:
    i32 window_height = -1;
    i32 window_width = -1;

    i32 grid_height = -1;
    i32 grid_width = -1;

    std::vector<std::pair<Location, EntityType>> entities;

    bool is_window_initialized = false;

    static constexpr i32 cell_size = 30;
    static constexpr i32 padding = 128;
  
  public:
    void render(const State &state) {
      [[maybe_unused]] static auto _ = [&] {
        grid_height = static_cast<int>(state.grid.size());
        grid_width = static_cast<int>(state.grid[0].size());

        window_height = grid_height * cell_size + 2 * padding;
        window_width = grid_width * cell_size + 2 * padding;

        entities.reserve(grid_height * grid_width);

        SetTraceLogLevel(LOG_WARNING);
        InitWindow(window_width, window_height, "Pacman RL");
        SetTargetFPS(60);

        is_window_initialized = true;

        return 0;
      }();

      entities.clear();
      for (i32 i = 0; i < grid_height; ++i)
        for (i32 j = 0; j < grid_width; ++j)
          entities.emplace_back(Location{i, j}, char_to_entity_type(state.grid[i][j]));
      std::sort(entities.begin(), entities.end(), [](const auto &a, const auto &b) {
        return entity_type_render_precedence(a.second) < entity_type_render_precedence(b.second);
      });
      
      BeginDrawing();
      ClearBackground(LIGHTBLACK);

      DrawText(TextFormat("Lives: %d", state.lives), 10, 10, 20, GREEN);
      DrawText(TextFormat("Step: %d", state.step_index), 10, 40, 20, GREEN);
      DrawText(TextFormat("Score: %d", state.score), 10, 70, 20, GREEN);
      DrawFPS(10, 100);

      for (const auto &[location, entity_type]: entities) {
        i32 pos_x = window_width / 2 - grid_width * cell_size / 2 + location.y * cell_size;
        i32 pos_y = window_height / 2 - grid_height * cell_size / 2 + location.x * cell_size;
        ::draw_entity(entity_type, pos_x, pos_y, cell_size);
      }

      EndDrawing();
    }

    void close() const {
      if (is_window_initialized)
        CloseWindow();
    }
};

#undef LIGHTBLACK

#endif // RENDER_GRAPHICS_RENDERER_H
