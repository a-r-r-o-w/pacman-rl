#include "environment.hpp"

Environment::Environment(const Config &c):
  config(c),
  state({
    .completed = false,
    .step_index = 0,
    .score = 0,
    .grid = std::vector <std::string> (config.rows, std::string(config.cols, ' '))
  }),
  grid(config.rows, config.cols) {
  sync_ghost_config();

  pacman = std::make_unique<Pacman>(Location{}, default_movement_direction[EntityType::pacman]);
  blinky = std::make_unique<Blinky>(config.blinky_config);
  pinky  = std::make_unique<Pinky>(config.pinky_config, config.pinky_target_offset);
  inky   = std::make_unique<Inky>(config.inky_config);
  clyde  = std::make_unique<Clyde>(config.clyde_config, config.clyde_target_switch_distance);
  dynamic_storage.emplace_back(entity_type_render_precedence[pacman->type], pacman.get());
  dynamic_storage.emplace_back(entity_type_render_precedence[blinky->type], blinky.get());
  dynamic_storage.emplace_back(entity_type_render_precedence[pinky->type], pinky.get());
  dynamic_storage.emplace_back(entity_type_render_precedence[inky->type], inky.get());
  dynamic_storage.emplace_back(entity_type_render_precedence[clyde->type], clyde.get());

  initialize_grid();
  std::sort(dynamic_storage.begin(), dynamic_storage.end());
  assign_state_grid_from_grid();
}

Environment::~Environment() {
  
}

void Environment::render(const RenderMode &mode) {
  if (mode == RenderMode::stdout)
    ascii_renderer.render(state);
  else
    throw std::runtime_error("Only RenderMode::stdout support for now!");
}

State Environment::step(const MovementDirection &direction) {
  const auto blinky_target = blinky->get_target(pacman.get());
  const auto pinky_target  = pinky->get_target(pacman.get());
  const auto inky_target   = inky->get_target(pacman.get(), blinky.get());
  const auto clyde_target  = clyde->get_target(pacman.get());

  // std::cout << blinky->direction << ' ' << pinky->direction << ' ' << inky->direction << ' ' << clyde->direction << '\n';
  // std::cout << blinky->config.mode << ' ' << pinky->config.mode << ' ' << inky->config.mode << ' ' << clyde->config.mode << '\n';
  // std::cout << "Blinky: " << blinky->location.x << " " << blinky->location.y << ' ' << blinky->config.mode << "\n";
  // std::cout << "Blinky: " << blinky_target.x << " " << blinky_target.y << "\n";
  // std::cout << "Pinky: " << pinky_target.x << " " << pinky_target.y << "\n";
  // std::cout << "Inky: " << inky_target.x << " " << inky_target.y << "\n";
  // std::cout << "Clyde: " << clyde_target.x << " " << clyde_target.y << "\n";
  
  auto [pacman_location, pacman_direction] = perform_pacman_step(direction);
  auto [blinky_location, blinky_direction] = perform_ghost_step(blinky.get(), blinky_target);
  auto [pinky_location, pinky_direction]   = perform_ghost_step(pinky.get(), pinky_target);
  auto [inky_location, inky_direction]     = perform_ghost_step(inky.get(), inky_target);
  auto [clyde_location, clyde_direction]   = perform_ghost_step(clyde.get(), clyde_target);
// std::cout << "Blinky: " << blinky_location.x << " " << blinky_location.y << "\n";

  update_score(pacman_location);
  
  for (auto &[_, entity]: dynamic_storage)
    unset_grid(entity);

  pacman->set(pacman_location, pacman_direction);
  blinky->step(blinky_location, blinky_direction);
  pinky->step(pinky_location, pinky_direction);
  inky->step(inky_location, inky_direction);
  clyde->step(clyde_location, clyde_direction);

  for (auto &[_, entity]: dynamic_storage)
    set_grid(entity);

  state.step_index += 1;
  if (state.step_index >= config.max_episode_steps)
    state.completed = true;
  assign_state_grid_from_grid();
  return state;
}

std::pair <Location, MovementDirection> Environment::perform_pacman_step(const MovementDirection &direction) {
  i32 nx = pacman->location.x + movement_direction_delta_x[direction];
  i32 ny = pacman->location.y + movement_direction_delta_y[direction];

  if (is_valid_pacman_move({nx, ny}))
    return {Location{nx, ny}, direction};
  
  return {Location{pacman->location.x, pacman->location.y}, pacman->direction};
}

std::pair <Location, MovementDirection> Environment::perform_ghost_step(Ghost *ghost, Location target) {
  if (ghost->config.mode == GhostMode::house)
    return {ghost->location, MovementDirection::none};
  
  // TODO: Blinky's initial position is used as the target when a ghost moves out of the
  // house. This is not the correct behaviour since Blinky could start from any position
  // on an arbitrary map. Ideally, some position next to the gate should be used as target.
  if (ghost->house_state_updated) {
    if (ghost->location.x == blinky->config.initial_location.x and ghost->location.y == blinky->config.initial_location.y)
      ghost->house_state_updated = false;
    target = blinky->config.initial_location;
  }

  if (ghost->config.mode == GhostMode::freight) {
    // TODO: Here, it is hardcoded that if the ghost is in freight mode, updates
    // will happen every 2 steps. Think of the correct way of handling this.
    if (ghost->config.step_index % 2 == 0)
      return {ghost->location, ghost->direction};
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

    if (is_valid_ghost_move(ghost, {nx, ny})) {
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

    if (is_valid_ghost_move(ghost, {nx, ny})) {
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

bool Environment::is_valid_pacman_move(const Location &location) {
  Entity* entity = grid.get(location);
  return (entity == nullptr or (entity->type != EntityType::wall and entity->type != EntityType::gate));
}

bool Environment::is_valid_ghost_move(Ghost *ghost, const Location &location) {
  Entity* entity = grid.get(location);
  if (entity == nullptr)
    return true;
  if (entity->type == EntityType::gate)
    return ghost->house_state_updated;
  return entity->type != EntityType::wall;
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
          entities.emplace_back(std::make_unique<Wall>(location));
          Wall *wall = static_cast<Wall*>(entities.back().get());
          set_grid(wall);
          dynamic_storage.emplace_back(entity_type_render_precedence[wall->type], wall);
        }
        break;

        case entity_type_to_char[EntityType::gate]: {
          entities.emplace_back(std::make_unique<Gate>(location));
          Gate *gate = static_cast<Gate*>(entities.back().get());
          set_grid(gate);
          dynamic_storage.emplace_back(entity_type_render_precedence[gate->type], gate);
        }
        break;

        case entity_type_to_char[EntityType::pellet]: {
          entities.emplace_back(std::make_unique<Item>(EntityType::pellet, location, config.pellet_points));
          Item *item = static_cast<Item*>(entities.back().get());
          set_grid(item);
          dynamic_storage.emplace_back(entity_type_render_precedence[item->type], item);
        }
        break;

        case entity_type_to_char[EntityType::power_pellet]: {
          entities.emplace_back(std::make_unique<Item>(EntityType::power_pellet, location, config.power_pellet_points));
          Item *item = static_cast<Item*>(entities.back().get());
          set_grid(item);
          dynamic_storage.emplace_back(entity_type_render_precedence[item->type], item);
        }
        break;

        case entity_type_to_char[EntityType::pacman]:
          pacman->set(location, default_movement_direction[pacman->type]);
          set_grid(pacman.get());
          break;
        
        case entity_type_to_char[EntityType::blinky]:
          set_grid(blinky.get());
          break;
        
        case entity_type_to_char[EntityType::pinky]:
          set_grid(pinky.get());
          break;
        
        case entity_type_to_char[EntityType::inky]:
          set_grid(inky.get());
          break;
        
        case entity_type_to_char[EntityType::clyde]:
          set_grid(clyde.get());
          break;
        
        default:
          break;
      }
    }
  }
}

void Environment::assign_state_grid_from_grid() {
  Location location;
    
  for (i32 x = 0; x < config.rows; ++x) {
    location.x = x;
    for (i32 y = 0; y < config.cols; ++y) {
      location.y = y;
      Entity* entity = grid.get(location);
      if (entity == nullptr)
        state.grid[x][y] = ' ';
      else
        state.grid[x][y] = entity_type_to_char[entity->type];
    }
  }
}

void Environment::update_score(const Location &location) {
  Entity *entity = grid.get(location);
  if (entity == nullptr)
    return;
  if (entity->type == EntityType::pellet or entity->type == EntityType::power_pellet) {
    Item *item = static_cast<Item*>(entity);
    state.score += item->points;
    
    // TODO: This has been hardcoded as of now. Find a better way to do this.
    i32 index = -1;
    for (i32 i = 0; i < (i32)dynamic_storage.size(); ++i) {
      auto &[_, entity] = dynamic_storage[i];
      if (entity == item) {
        index = i;
        break;
      }
    }
    if (index != -1) {
      dynamic_storage.erase(dynamic_storage.begin() + index);
    }
    else
      throw std::runtime_error("This should not be possible.");
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
