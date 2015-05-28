set(CMAKE_INCLUDE_CURRENT_DIR ON)
ADD_EXECUTABLE(mockup_light
  main.c
)

message(STATUS ${MOCKUPLIB_INCLUDE_DIR})
message(STATUS ${R65816_INCLUDE_DIR})

target_include_directories(mockup_light PUBLIC ${R65816_INCLUDE_DIR})
target_include_directories(mockup_light PUBLIC ${MOCKUPLIB_INCLUDE_DIR})

target_link_libraries(mockup_light r65816)
target_link_libraries(mockup_light libmockup)
