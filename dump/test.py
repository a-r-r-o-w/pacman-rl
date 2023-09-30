import os
import random
import time

import build.pacman_rl as pacman_rl

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

# random.seed(0)
sleep_ms = 0.1
env = pacman_rl.make(config, pacman_rl.RenderMode.HUMAN)
state = pacman_rl.State()

moves = [
    pacman_rl.MovementDirection.right,
    pacman_rl.MovementDirection.left,
    pacman_rl.MovementDirection.up,
    pacman_rl.MovementDirection.down,
]

env.render()

move = moves[0]

while not state.completed:
    old_pacman_location = state.pacman_location
    state = env.step(move)
    if state.pacman_location.x == old_pacman_location.x and state.pacman_location.y == old_pacman_location.y:
        move = random.choice(moves)
    env.render()
    time.sleep(sleep_ms)

env.close()
