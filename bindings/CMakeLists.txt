include_directories(./)

pybind11_add_module(
  ${PYBIND_BINDING_FILE}
    ./pacman_bindings.cpp
)

set_target_properties(${PYBIND_BINDING_FILE} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

if (UNIX AND NOT APPLE)
  set(LINUX true)
endif()

if (LINUX)
  message(STATUS "Platform: Linux")

  target_link_libraries(
    ${PYBIND_BINDING_FILE}
    PUBLIC
      dl
      raylib
  )
elseif (WIN32)
  message(STATUS "Platform: Windows")

  target_link_libraries(
    ${PYBIND_BINDING_FILE}
    PUBLIC
      raylib
  )
endif()
