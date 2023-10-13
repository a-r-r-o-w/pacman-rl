#include <iostream>
#include <cstdlib>
#include <memory>
#include <ostream>
#include <thread>

#include "environment.hpp"
#include "state.hpp"
#include "render/render_utils.hpp"
#include "wrappers/record_video_env.hpp"

i32 sleep_ms = 50;
Config config = {
  .rows = 21,
  .cols = 19,
  .max_episode_steps = 100,
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
  .pacman_lives = 3,
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  srand(time(0));
  PacmanEnvironment p(config, RenderMode::human);
  RecordVideoEnvironment e(p, true, 24, "recordings", "recording.mp4");

  std::vector <MovementDirection> moves = {
    MovementDirection::left, MovementDirection::up, MovementDirection::right, MovementDirection::down
  };

  std::ignore = system("clear");
  e.render();
  render_grid_to_png(e.get_state().grid);
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
    // std::ignore = system("clear");
    state = e.step(moves[rand() % 4]);
    e.render();
    std::flush(std::cout);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
  }

  e.close();

  return 0;
}
