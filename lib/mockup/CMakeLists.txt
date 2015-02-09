add_library(libmockup
  decode.c
  smw.c
  gfx_store.c
  layer.c
  level.c
  object.c
  palette.c
  tiles.c
  tileset.c
)

target_include_directories(libmockup PUBLIC ${R65816_INCLUDE_DIR})
find_path(MOCKUPLIB_INCLUDE_DIR smw.h HINTS ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libmockup r65816)
