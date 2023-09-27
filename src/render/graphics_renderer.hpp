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

#define CYAN (Color{0, 255, 255, 255})
#define LIGHTBLACK (Color{24, 24, 24, 128})

class GraphicsRenderer {
  private:
    i32 window_height = -1;
    i32 window_width = -1;

    i32 grid_height = -1;
    i32 grid_width = -1;

    std::vector<std::pair<Location, EntityType>> entities;

    static constexpr i32 cell_size = 30.0f;
    static constexpr i32 padding = 128.0f;
  
  public:
    ~GraphicsRenderer() {
      CloseWindow();
    }

    void render(const State &state) {
      [[maybe_unused]] static auto _ = [&] {
        grid_height = static_cast<int>(state.grid.size());
        grid_width = static_cast<int>(state.grid[0].size());

        window_height = grid_height * cell_size + 2 * padding;
        window_width = grid_width * cell_size + 2 * padding;

        entities.reserve(grid_height * grid_width);

        InitWindow(window_width, window_height, "Pacman RL");
        SetTargetFPS(60);

        return 0;
      } ();

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
        draw_entity(entity_type, pos_x, pos_y);
      }

      EndDrawing();
    }

  private:
    // blinky, pinky, inky, clyde -> circle
    // pacman -> circle
    // wall -> square
    // gate -> square
    // pellet -> 8-sided circle, smaller in size
    // power_pellet -> 4-sided circle, bigger in size
    void draw_entity(EntityType type, int pos_x, int pos_y) const {
      switch (type) {
        case EntityType::blinky:
        case EntityType::pinky:
        case EntityType::inky:
        case EntityType::clyde:
        case EntityType::pacman:
          DrawCircle(pos_x + cell_size / 2, pos_y + cell_size / 2, (f32)cell_size / 2, get_color_for_entity_type(type));
          break;
        
        case EntityType::wall:
        case EntityType::gate:
          DrawRectangle(pos_x, pos_y, cell_size, cell_size, get_color_for_entity_type(type));
          break;
        
        case EntityType::pellet:
          DrawCircle(pos_x + (f32)cell_size / 2, pos_y + (f32)cell_size / 2, (f32)cell_size / 8, get_color_for_entity_type(type));
          break;
        
        case EntityType::power_pellet:
          DrawCircleSector(Vector2{pos_x + (f32)cell_size / 2, pos_y + (f32)cell_size / 2}, (f32)cell_size / 3, 0, 360, 8, get_color_for_entity_type(type));
          break;
        
        default:
          break;
      }
    }

    Color get_color_for_entity_type(EntityType type) const {
      switch (type) {
        case EntityType::blinky:
          return RED;
        case EntityType::pinky:
          return PINK;
        case EntityType::inky:
          return CYAN;
        case EntityType::clyde:
          return ORANGE;
        case EntityType::pacman:
          return YELLOW;
        case EntityType::wall:
          return BLUE;
        case EntityType::gate:
          return BROWN;
        case EntityType::pellet:
          return LIGHTGRAY;
        case EntityType::power_pellet:
          return WHITE;
        default:
          return BLACK;
      }
    }
};

#endif // RENDER_GRAPHICS_RENDERER_H
