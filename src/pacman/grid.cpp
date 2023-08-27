#include <vector>

#include "pacman/grid.hpp"

Grid::Grid(i32 rows, i32 cols):
  rows(rows),
  cols(cols) {
  map.resize(rows * cols, nullptr);
}

Grid::~Grid() {
  map.clear();
}

i32 Grid::get_index(const Location &location) {
  return location.x * cols + location.y;
}

void Grid::reset() {
  map.assign(map.size(), nullptr);
}

Entity* Grid::get(const Location &location) {
  return map[get_index(location)];
}

void Grid::set(const Location &location, Entity *e) {
  map[get_index(location)] = e;
}

void Grid::unset(const Location &location) {
  map[get_index(location)] = nullptr;
}
