set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("${CMAKE_SOURCE_DIR}/lib")
include_directories("${CMAKE_SOURCE_DIR}/lib/bsnes")

set_property(TARGET bsnes-r65816 PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/bsnes/obj/processor-r65816.o)
add_library(libmockup
  rom.c
  cpu/cpu.c
  cpu/memory.c
  cpu/opcode_pc.c
  cpu/opcode_misc.c
  cpu/algorithms.c
)
set_target_properties(libmockup PROPERTIES PREFIX "")
target_link_libraries(libmockup ${CMAKE_SOURCE_DIR}/bsnes/obj/processor-r65816.o)
