add_executable(cputest
  main.cpp
  )

add_custom_command(
  OUTPUT ${PROJECT_SOURCE_DIR}/bsnes/obj/processor-r65816.o
  COMMAND make obj/processor-r65816.o
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bsnes
  )
add_custom_target(
  bsnes-r65816
  DEPENDS ${CMAKE_SOURCE_DIR}/bsnes/obj/processor-r65816.o
  )

target_include_directories(cputest PUBLIC ${R65816_INCLUDE_DIR} bsnes)
target_link_libraries(cputest PUBLIC r65816)

add_test(NAME test_cpu COMMAND $<TARGET_FILE:cputest>)
