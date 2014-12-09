add_library(libmockup
  level.c
  tiles.c
)

target_include_directories(libmockup PUBLIC ${R65816_INCLUDE_DIR})
