#ifndef PACMAN_GRID_H
#define PACMAN_GRID_H
#pragma once

#include <vector>

#include "pacman/entity.hpp"
#include "types.hpp"

class Grid {
  public:
    i32 rows;
    i32 cols;
    std::vector <Entity*> map;
  
  private:
    i32 get_index(const Location &location) {
      return location.x * cols + location.y;
    }
  
  public:
    Grid(i32 rows, i32 cols):
      rows(rows),
      cols(cols) {
      map.resize(rows * cols, nullptr);
    }

    ~Grid() {
      map.clear();
    }

    void reset() {
      map.assign(map.size(), nullptr);
    }

    Entity* get(const Location &location) {
      return map[get_index(location)];
    }

    void set(const Location &location, Entity *e) {
      map[get_index(location)] = e;
    }

    void unset(const Location &location) {
      map[get_index(location)] = nullptr;
    }
};

#endif // PACMAN_GRID_H
