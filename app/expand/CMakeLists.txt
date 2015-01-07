set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(expand
  main.c
  )

target_include_directories(expand PUBLIC ${R65816_INCLUDE_DIR})
target_link_libraries(expand r65816)
