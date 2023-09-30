#include <iostream>
#include <cstdlib>
#include <ostream>
#include <thread>

#include "environment.hpp"
#include "state.hpp"

i32 sleep_ms = 50;
Config config = {
  .rows = 21,
  .cols = 19,
  .max_episode_steps = 1000,
  .map = {
    "###################",
    "#........#........#",
    "#@##.###.#.###.##@#",
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
    "#@.#...........#.@#",
    "##.#.#.#####.#.#.##",
    "#....#...#...#....#",
    "#.######.#.######.#",
    "#.................#",
    "###################",
  },
  .pacman_lives = 20,
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  srand(time(0));
  // Environment e(config, RenderMode::human);
  Environment e = std::move(make(config, RenderMode::human));

  std::vector <MovementDirection> moves = {
    MovementDirection::left, MovementDirection::up, MovementDirection::right, MovementDirection::down
  };

  // std::ignore = system("clear");
  e.render();
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

  State state = e.reset();
  // int episodes = 100;

  // for (int i = 0; i < episodes; ++i) {
  //   state = e.reset();
  //   while (not state.completed)
  //     state = e.step(moves[rand() % 4]);
  //   std::cout << "episode " << i << " score: " << state.score << ", steps: " << state.step_index << std::endl;
  // }
  
  while (not state.completed) {
    std::ignore = system("clear");
    state = e.step(moves[rand() % 4]);
    e.render();
    std::flush(std::cout);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
  }

  return 0;
}
