#include <iostream>
#include <pybind11/cast.h>
#include <pybind11/detail/common.h>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "environment.hpp"
#include "pretty_print.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pacman_rl, m) {
  m.doc() = "Pacman environment for Reinforcement Learning";

  py::class_<Location>(m, "Location")
    .def(py::init<>(), "Default constructor")
    .def(py::init<i32, i32>(), "Constructor with x and y coordinates")
    .def_readwrite("x", &Location::x, "x coordinate")
    .def_readwrite("y", &Location::y, "y coordinate")
    .def("__repr__", [](const Location &) { return "<pacman_rl.Location>"; })
    .def("pretty", pretty_location);

  py::enum_<MovementDirection>(m, "MovementDirection")
    .value("up", MovementDirection::up, "Up direction")
    .value("left", MovementDirection::left, "Left direction")
    .value("down", MovementDirection::down, "Down direction")
    .value("right", MovementDirection::right, "Right direction")
    .value("none", MovementDirection::none, "No direction")
    .export_values();
  
  py::class_<GhostConfig>(m, "GhostConfig")
    .def(py::init<>(), "Default constructor")
    .def(py::init<i32, i32, i32, i32, i32, MovementDirection, Location, Location, GhostMode>(), "Constructor with all attributes")
    .def_readwrite("step_index", &GhostConfig::step_index, "Current step index in the current mode")
    .def_readwrite("chase_steps", &GhostConfig::chase_steps, "Number of steps in chase mode before mode switch")
    .def_readwrite("scatter_steps", &GhostConfig::scatter_steps, "Number of steps in scatter mode before mode switch")
    .def_readwrite("freight_steps", &GhostConfig::freight_steps, "Number of steps in freight mode before mode switch")
    .def_readwrite("house_steps", &GhostConfig::house_steps, "Number of steps ghost stays in house before mode switch")
    .def_readwrite("initial_direction", &GhostConfig::initial_direction, "Initial direction of ghost")
    .def_readwrite("initial_location", &GhostConfig::initial_location, "Initial location of ghost")
    .def_readwrite("corner", &GhostConfig::corner, "Corner location of ghost")
    .def_readwrite("mode", &GhostConfig::mode, "Ghost mode (chase, scatter, freight, eaten, house)")
    .def("__repr__", [](const GhostConfig &) { return "<pacman_rl.GhostConfig>"; })
    .def("pretty", pretty_ghost_config);
  
  py::class_<Config>(m, "Config")
    .def(py::init<>(), "Default constructor")
    .def(py::init<i32, i32, i32, std::vector <std::string>, GhostConfig, GhostConfig, GhostConfig, GhostConfig, i32, i32, i32, i32, i32, i32>(), "Constructor with all attributes")
    .def_readwrite("rows", &Config::rows, "Number of rows in the grid")
    .def_readwrite("cols", &Config::cols, "Number of columns in the grid")
    .def_readwrite("max_episode_steps", &Config::max_episode_steps, "Maximum number of steps per episode")
    .def_readwrite("map", &Config::map, "Map of the grid")
    .def_readwrite("blinky_config", &Config::blinky_config, "Blinky ghost config")
    .def_readwrite("pinky_config", &Config::pinky_config, "Pinky ghost config")
    .def_readwrite("inky_config", &Config::inky_config, "Inky ghost config")
    .def_readwrite("clyde_config", &Config::clyde_config, "Clyde ghost config")
    .def_readwrite("pacman_lives", &Config::pacman_lives, "Number of lives pacman has")
    .def_readwrite("score_per_ghost_eaten", &Config::score_per_ghost_eaten, "Score for eating a ghost")
    .def_readwrite("pinky_target_offset", &Config::pinky_target_offset, "Offset from pacman's location for pinky's target")
    .def_readwrite("clyde_target_switch_distance", &Config::clyde_target_switch_distance, "Distance from pacman's location for clyde to switch target")
    .def_readwrite("pellet_points", &Config::pellet_points, "Points for eating a pellet")
    .def_readwrite("power_pellet_points", &Config::power_pellet_points, "Points for eating a power pellet")
    .def_readwrite("power_pellet_steps", &Config::power_pellet_steps, "Number of steps for which the effect of power pellet lasts")
    .def("__repr__", [](const Config &) { return "<pacman_rl.Config>"; })
    .def("pretty", pretty_config);
  
  py::enum_<RenderMode>(m, "RenderMode")
    .value("STDOUT", RenderMode::stdout, "Render to stdout");

  py::class_<State>(m, "State")
    .def(py::init<>(), "Default constructor")
    .def_readwrite("step_index", &State::step_index, "Current step index")
    .def_readwrite("score", &State::score, "Current score")
    .def_readwrite("lives", &State::lives, "Current number of lives")
    .def_readwrite("completed", &State::completed, "Whether the episode is completed")
    .def_readwrite("pacman_location", &State::pacman_location, "Current pacman location")
    .def_readwrite("blinky_location", &State::blinky_location, "Current blinky location")
    .def_readwrite("pinky_location", &State::pinky_location, "Current pinky location")
    .def_readwrite("inky_location", &State::inky_location, "Current inky location")
    .def_readwrite("clyde_location", &State::clyde_location, "Current clyde location")
    .def_readwrite("grid", &State::grid, "Current grid")
    .def("__repr__", [](const State &) { return "<pacman_rl.State>"; })
    .def("pretty", pretty_state);
  
  py::class_<Environment>(m, "Environment")
    .def(py::init<Config>(), "Constructor with config")
    .def("reset", &Environment::reset, "Reset the environment")
    .def("step", &Environment::step, "Perform an action in the environment")
    .def("render", &Environment::render, py::arg("mode") = RenderMode::stdout, "Render the environment")
    .def("__repr__", [](const Environment &) { return "<pacman_rl.Environment>"; })
    .def("pretty", pretty_environment, "Pretty print the environment");
  
  m.def("make_env", &make_env, py::return_value_policy::move, "Creates and returns an environment with the given config");
  
  m.def_submodule("pretty_print")
    .def("pretty_location", pretty_location, "Pretty print a location")
    .def("pretty_ghost_config", pretty_ghost_config, "Pretty print a ghost config")
    .def("pretty_config", pretty_config, "Pretty print a config")
    .def("pretty_state", pretty_state, "Pretty print a state")
    .def("pretty_environment", pretty_environment, "Pretty print an environment");
};
