#include <iostream>
#include <queue>

#include "environment.hpp"

Environment::Environment(const Config &c):
  config(c),
  state({
    .completed = false,
    .step_index = 0
  }),
  grid(config.rows, config.cols) {
  sync_ghost_config();

  pacman = new Pacman({}, default_movement_direction[EntityType::pacman]);
  blinky = new Blinky(config.blinky_config);
  pinky  = new Pinky(config.pinky_config, config.pinky_target_offset);
  inky   = new Inky(config.inky_config);
  clyde  = new Clyde(config.clyde_config, config.clyde_target_switch_distance);
  dynamic_storage.emplace_back(pacman);
  dynamic_storage.emplace_back(blinky);
  dynamic_storage.emplace_back(pinky);
  dynamic_storage.emplace_back(inky);
  dynamic_storage.emplace_back(clyde);

  initialize_grid();
}

Environment::~Environment() {
  while (!dynamic_storage.empty()) {
    Entity* entity = dynamic_storage.back();
    dynamic_storage.pop_back();
    delete entity;
  }
}

void Environment::render(const RenderMode &mode) {
  if (mode == RenderMode::stdout) {
    Location location;
    
    std::cout << "Step: " << state.step_index << "\n\n";
    for (i32 x = 0; x < config.rows; ++x) {
      location.x = x;
      for (i32 y = 0; y < config.cols; ++y) {
        location.y = y;
        Entity* entity = grid.get(location);
        if (entity == nullptr)
          std::cout << " ";
        else
          std::cout << entity_type_to_char[entity->type];
      }
      std::cout << '\n';
    }
    std::cout << std::endl;
  }
}

State Environment::step(const MovementDirection &direction) {
  const auto blinky_target = blinky->get_target(pacman);
  const auto pinky_target = pinky->get_target(pacman);
  const auto inky_target = inky->get_target(pacman, blinky);
  const auto clyde_target = clyde->get_target(pacman);

  // std::cout << blinky->direction << ' ' << pinky->direction << ' ' << inky->direction << ' ' << clyde->direction << '\n';
  // std::cout << blinky->config.mode << ' ' << pinky->config.mode << ' ' << inky->config.mode << ' ' << clyde->config.mode << '\n';
  // std::cout << "Blinky: " << blinky->location.x << " " << blinky->location.y << ' ' << blinky->config.mode << "\n";
  // std::cout << "Blinky: " << blinky_target.x << " " << blinky_target.y << "\n";
  // std::cout << "Pinky: " << pinky_target.x << " " << pinky_target.y << "\n";
  // std::cout << "Inky: " << inky_target.x << " " << inky_target.y << "\n";
  // std::cout << "Clyde: " << clyde_target.x << " " << clyde_target.y << "\n";
  
  auto [pacman_location, pacman_direction] = perform_pacman_step(direction);
  auto [blinky_location, blinky_direction] = perform_ghost_step(blinky, blinky_target);
  auto [pinky_location, pinky_direction] = perform_ghost_step(pinky, pinky_target);
  auto [inky_location, inky_direction] = perform_ghost_step(inky, inky_target);
  auto [clyde_location, clyde_direction] = perform_ghost_step(clyde, clyde_target);
// std::cout << "Blinky: " << blinky_location.x << " " << blinky_location.y << "\n";

  for (Entity *entity: dynamic_storage)
    unset_grid(entity);

  pacman->set(pacman_location, pacman_direction);
  blinky->step(blinky_location, blinky_direction);
  pinky->step(pinky_location, pinky_direction);
  inky->step(inky_location, inky_direction);
  clyde->step(clyde_location, clyde_direction);

  for (Entity *entity: dynamic_storage)
    set_grid(entity);

  state.step_index += 1;
  if (state.step_index >= config.max_episode_steps)
    state.completed = true;
  return state;
}

std::pair <Location, MovementDirection> Environment::perform_pacman_step(const MovementDirection &direction) {
  i32 nx = pacman->location.x + movement_direction_delta_x[direction];
  i32 ny = pacman->location.y + movement_direction_delta_y[direction];

  if (is_valid_move({nx, ny}))
    return {Location{nx, ny}, direction};
  
  return {Location{pacman->location.x, pacman->location.y}, pacman->direction};
}

std::pair <Location, MovementDirection> Environment::perform_ghost_step(Ghost *ghost, const Location &target) {
  if (ghost->config.mode == GhostMode::house)
    return {ghost->location, MovementDirection::none};
  
  if (ghost->house_state_updated) {
    ghost->house_state_updated = false;
    return {blinky->config.initial_location, MovementDirection::none};
  }
  
  i32 best_x = ghost->location.x, best_y = ghost->location.y, best_distance = i32_inf;
  MovementDirection best_direction = ghost->direction;
  bool has_valid_move = false;

  for (const MovementDirection &direction: movement_direction_precedence) {
  // for (const MovementDirection &direction: legal_ghost_movement_direction[ghost->direction]) {
    if (direction == opposite_direction[ghost->direction])
      continue;
    
    i32 nx = ghost->location.x + movement_direction_delta_x[direction];
    i32 ny = ghost->location.y + movement_direction_delta_y[direction];

    if (is_valid_move({nx, ny})) {
      has_valid_move = true;
      i32 distance = manhattan_distance(nx, ny, target.x, target.y);
      if (distance < best_distance) {
        best_distance = distance;
        best_x = nx;
        best_y = ny;
        best_direction = direction;
      }
    }
  }

  if (not has_valid_move) {
    MovementDirection direction = opposite_direction[ghost->direction];
    i32 nx = ghost->location.x + movement_direction_delta_x[direction];
    i32 ny = ghost->location.y + movement_direction_delta_y[direction];

    if (is_valid_move({nx, ny})) {
      i32 distance = manhattan_distance(nx, ny, target.x, target.y);
      if (distance < best_distance) {
        best_x = nx;
        best_y = ny;
        best_direction = direction;
      }
    }
  }

  return {Location{best_x, best_y}, best_direction};
}

bool Environment::is_valid_move(const Location &location) {
  Entity* entity = grid.get(location);
  return (entity == nullptr or (entity->type != EntityType::wall and entity->type != EntityType::gate));
}

void Environment::initialize_grid() {
  Location location;
  for (i32 x = 0; x < config.rows; ++x) {
    location.x = x;
    for (i32 y = 0; y < config.cols; ++y) {
      location.y = y;
      char c = config.map[x][y];

      switch(c) {
        case entity_type_to_char[EntityType::wall]: {
          Wall *wall = new Wall(location);
          set_grid(wall);
          dynamic_storage.emplace_back(wall);
        }
        break;

        case entity_type_to_char[EntityType::gate]: {
          Gate *gate = new Gate(location);
          set_grid(gate);
          dynamic_storage.emplace_back(gate);
        }
        break;

        case entity_type_to_char[EntityType::pacman]:
          pacman->set(location, default_movement_direction[pacman->type]);
          set_grid(pacman);
          break;
        
        case entity_type_to_char[EntityType::blinky]:
          set_grid(blinky);
          break;
        
        case entity_type_to_char[EntityType::pinky]:
          set_grid(pinky);
          break;
        
        case entity_type_to_char[EntityType::inky]:
          set_grid(inky);
          break;
        
        case entity_type_to_char[EntityType::clyde]:
          set_grid(clyde);
          break;
        
        default:
          break;
      }
    }
  }
}

void Environment::sync_ghost_config() {
  config.blinky_config.corner = Location{-2, config.cols - 2};
  config.pinky_config.corner  = Location{-2, +2};
  config.inky_config.corner   = Location{config.rows + 1, config.cols - 1};
  config.clyde_config.corner  = Location{config.rows + 1, +1};

  Location location;
  for (i32 x = 0; x < config.rows; ++x) {
    location.x = x;
    for (i32 y = 0; y < config.cols; ++y) {
      location.y = y;
      char c = config.map[x][y];

      switch(c) {
        case entity_type_to_char[EntityType::blinky]:
          config.blinky_config.initial_location = location;
          break;
        
        case entity_type_to_char[EntityType::pinky]:
          config.pinky_config.initial_location = location;
          break;
        
        case entity_type_to_char[EntityType::inky]:
          config.inky_config.initial_location = location;
          break;
        
        case entity_type_to_char[EntityType::clyde]:
          config.clyde_config.initial_location = location;
          break;
        
        default:
          break;
      }
    }
  }
}

void Environment::unset_grid(Entity* entity) {
  grid.unset(entity->location);
}

void Environment::set_grid(Entity* entity) {
  grid.set(entity->location, entity);
}
