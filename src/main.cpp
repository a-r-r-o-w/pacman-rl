#include <iostream>
#include <cstdlib>
#include <thread>

#include "environment.hpp"

i32 sleep_ms = 200;
Config config = {
  .rows = 21,
  .cols = 20,
  .max_episode_steps = 200,
  .map = {
    "###################",
    "#........#........#",
    "#+##.###.#.###.##+#",
    "#.................#",
    "#.##.#.#####.#.##.#",
    "#....#...#...#....#",
    "####.###.#.###.####",
    "####.#...0...#.####",
    "####.#.##G##.#.####",
    "#......#123#......#",
    "####.#.#####.#.####",
    "####.#...P...#.####",
    "####.#.#####.#.####",
    "#........#........#",
    "#.##.###.#.###.##.#",
    "#+.#...........#.+#",
    "##.#.#.#####.#.#.##",
    "#....#...#...#....#",
    "#.######.#.######.#",
    "#.................#",
    "###################",
  }
};

State step(Environment &e, const MovementDirection &direction) {
  system("clear");
  State state = e.step(direction);
  // State state = e.step(MovementDirection::none);  
  e.render(RenderMode::stdout);
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
  return state;
}

int main() {
  // srand(time(0));
  Environment e(config);
  State state = {
    .completed = false,
    .step_index = 0,
  };

  std::vector <MovementDirection> moves = {
    MovementDirection::left, MovementDirection::up, MovementDirection::right, MovementDirection::down
  };
  int index = 0;

  system("clear");
  e.render(RenderMode::stdout);
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
  
  while (not state.completed) {
    // state = step(e, moves[index++ % moves.size()]);
    state = step(e, moves[rand() % moves.size()]);
  }

  return 0;
}
