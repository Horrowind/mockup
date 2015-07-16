add_library(asar
  arch-65816.cpp
  arch-superfx.cpp
  interface-cli.cpp
  libcon.cpp
  libstr.cpp
  main.cpp
  mathlib.cpp
  arch-spc700.cpp
  assembleblock.cpp
  interface-lib.cpp
  libsmw.cpp
  macro.cpp
  math.cpp
  )

find_path(ASARLIB_INCLUDE_DIR asar.h HINTS ${CMAKE_CURRENT_SOURCE_DIR})
