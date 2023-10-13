#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <iostream>
#include <string>
#include <vector>

#include "raylib.h"

#include "constants.hpp"
#include "types.hpp"
#include "pacman/state.hpp"

#define CYAN (Color{0, 255, 255, 255})
#define LIGHTBLACK (Color{24, 24, 24, 255})

inline Color get_color_for_entity_type(const EntityType type) {
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

inline void draw_entity(const EntityType type, const i32 pos_x, const i32 pos_y, const i32 cell_size) {
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
      DrawCircle(pos_x + (f32)cell_size / 2, pos_y + (f32)cell_size / 2, (f32)cell_size / 3, get_color_for_entity_type(type));
      break;
    
    default:
      break;
  }
}

inline void draw_entity(Image *image, const EntityType type, const i32 pos_x, const i32 pos_y, const i32 cell_size) {
  switch (type) {
    case EntityType::blinky:
    case EntityType::pinky:
    case EntityType::inky:
    case EntityType::clyde:
    case EntityType::pacman:
      ImageDrawCircle(image, pos_x + cell_size / 2, pos_y + cell_size / 2, (f32)cell_size / 2, get_color_for_entity_type(type));
      break;
    
    case EntityType::wall:
    case EntityType::gate:
      ImageDrawRectangle(image, pos_x, pos_y, cell_size, cell_size, get_color_for_entity_type(type));
      break;
    
    case EntityType::pellet:
      ImageDrawCircle(image, pos_x + (f32)cell_size / 2, pos_y + (f32)cell_size / 2, (f32)cell_size / 8, get_color_for_entity_type(type));
      break;
    
    case EntityType::power_pellet:
      ImageDrawCircle(image, pos_x + (f32)cell_size / 2, pos_y + (f32)cell_size / 2, (f32)cell_size / 3, get_color_for_entity_type(type));
      break;
    
    default:
      break;
  }
}

inline void render_grid_to_png(const std::vector<std::string> &grid, const std::string &filename = "tmp.png", const i32 cell_size = 30.0f, const i32 padding = 4.0f) {
  const i32 grid_height = static_cast<int>(grid.size());
  const i32 grid_width = static_cast<int>(grid[0].size());
  const i32 window_height = grid_height * cell_size + 2 * padding;
  const i32 window_width = grid_width * cell_size + 2 * padding;

  std::vector<std::pair<Location, EntityType>> entities;
  entities.reserve(grid_height * grid_width);
  
  for (i32 i = 0; i < grid_height; ++i)
    for (i32 j = 0; j < grid_width; ++j)
      entities.emplace_back(Location{i, j}, char_to_entity_type(grid[i][j]));
  std::sort(entities.begin(), entities.end(), [](const auto &a, const auto &b) {
    return entity_type_render_precedence(a.second) < entity_type_render_precedence(b.second);
  });

  bool new_window_created = false;

  if (not IsWindowReady()) {
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(1, 1, "Headless Window");
    new_window_created = true;
  }

  BeginDrawing();
  
  Image img = GenImageColor(window_width, window_height, LIGHTBLACK);
  ImageClearBackground(&img, LIGHTBLACK);
  
  for (const auto &[location, entity_type]: entities) {
    i32 pos_x = window_width / 2 - grid_width * cell_size / 2 + location.y * cell_size;
    i32 pos_y = window_height / 2 - grid_height * cell_size / 2 + location.x * cell_size;
    draw_entity(&img, entity_type, pos_x, pos_y, cell_size);
  }  

  EndDrawing();

  ExportImage(img, filename.c_str());
  UnloadImage(img);

  if (new_window_created)
    CloseWindow();
}

#undef CYAN
#undef LIGHTBLACK

#endif // RENDER_UTILS_H
