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
    i32 get_index(const Location &location);
  
  public:
    Grid(i32 rows, i32 cols);
    ~Grid();

    void reset();
    Entity* get(const Location &location);
    void set(const Location &location, Entity* e);
    void unset(const Location &location);
};

#endif // PACMAN_GRID_H
