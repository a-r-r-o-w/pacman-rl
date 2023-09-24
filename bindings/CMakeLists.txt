include_directories(./)

pybind11_add_module(
  "${PYBIND_BINDING_FILE}"
    ./pacman_bindings.cpp
)
