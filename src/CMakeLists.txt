include_directories(./render)
include_directories(./pacman)
include_directories(./)

add_executable(
  pacman-rl
    ./main.cpp
)

if (UNIX AND NOT APPLE)
  set(LINUX true)
endif()

if (LINUX)
  message(STATUS "Platform: Linux")

  target_link_libraries(
    pacman-rl
    PUBLIC
      dl
      raylib
  )
elseif (WIN32)
  message(STATUS "Platform: Windows")

  target_link_libraries(
    pacman-rl
    PUBLIC
      raylib
  )
endif()
