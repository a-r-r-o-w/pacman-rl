# PacmanRL

PacmanRL is a reinforcement learning environment for Pacman with an API similar to OpenAI Gym.

*Note: This is a WIP. The code is held together with duct tape and will probably break or not compile if you try to use it 🫠*

### Installation

##### Linux

You'll need to have installed [CMake](https://cmake.org/) before you can build PacmanRL.

```bash
git clone --recurse-submodules --depth 1 https://github.com/a-r-r-o-w/pacman-rl.git
cd pacman-rl
./build.sh -t RELEASE

# Tested on PopOS 22.04 LTS (NVIDIA)
# CMake 3.26.0
# GCC 11.3.0
```

*Yeah, no. That's probably not going to work since you require a few `apt install`'s before you can build PacmanRL. It should be easy to figure out based on the error messages though, right...? I wrote the initial version of this over a year ago and cannot recall all the packages required. Will update this soon.*

##### Windows

Nope. To be updated soon.

### Usage

<details>
  <summary> Python </summary>

```python
import random
import time

# I haven't figured out how to write a setup.py yet to properly install this
import build.pacman_rl as pacman_rl

# No comments 😔 I'm sorry, I'll improve this soon...
config = pacman_rl.Config()
config.rows = 21
config.cols = 19
config.max_episode_steps = 200
config.pacman_lives = 3
config.map = [
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
]

random.seed(42)

# Don't name the below variables the same.
# There's this weird bug that causes the game to crash if you do.
# A C++ destructor is called twice apparently, due to the difference in how
# C++ and Python handle memory management.
# As I mentioned, this is held together with duct tape.
pacman_env = pacman_rl.make(config, pacman_rl.RenderMode.HUMAN)
env = pacman_rl.RecordVideoEnvironment(pacman_env)
state = env.reset()

moves = [
    pacman_rl.MovementDirection.right,
    pacman_rl.MovementDirection.left,
    pacman_rl.MovementDirection.up,
    pacman_rl.MovementDirection.down,
]

while not state.completed:
    old_pacman_location = state.pacman_location
    move = random.choice(moves)
    state = env.step(move)
    env.render()
    time.sleep(0.1)

env.close()
```

</details>

<details>
  <summary> C++ </summary>

```cpp
#include <iostream>
#include <thread>
#include <vector>

// TODO: Make a single header file to include all of these
#include "types.hpp"
#include "environment.hpp"
#include "wrappers/record_video_env.hpp"

int main() {
  const i32 sleep_ms = 50;

  // Sorry, will work on making this better
  const Config config = {
    .rows = 21,
    .cols = 19,
    .max_episode_steps = 200,
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

  PacmanEnvironment p(config, RenderMode::human);
  RecordVideoEnvironment e(p);

  std::vector <MovementDirection> moves = {
    MovementDirection::left, MovementDirection::up, MovementDirection::right, MovementDirection::down
  };

  State state = e.reset();

  while (not state.completed) {
    state = e.step(moves[rand() % 4]);
    e.render();
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
  }

  e.close();

  return 0;
}
```
</details>

### Results

After training for not too many steps (my GPU was dying because of how unoptimized this is), here's an interesting run that demonstrates duct tape code in action, buggy implementation of the environment, and a Pacman that's not very good at playing Pacman.

<div align="center">
  <video src="https://github.com/a-r-r-o-w/pacman-rl/assets/72266394/c063757f-883e-408d-8aae-98005cdd24e0" controls autoplay> Your browser does not support the video tag. </video>
</div>

### License

MIT
