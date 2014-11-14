set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("${CMAKE_SOURCE_DIR}/lib")

add_library(libmockup
  level.c
  tiles.c
)
