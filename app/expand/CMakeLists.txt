set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(expand
  main.c
  )

target_include_directories(expand PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(expand r65816)
