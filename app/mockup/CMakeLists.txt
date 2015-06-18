
FIND_PACKAGE(Qt5Widgets)
if(Qt5Widgets_FOUND)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  set(CMAKE_AUTOMOC ON)
  ADD_EXECUTABLE(mockup
    main.cpp
    main_window.cpp
    qtiled_graphics_scene.cpp
    )
  message(STATUS ${MOCKUPLIB_INCLUDE_DIR})
  message(STATUS ${R65816_INCLUDE_DIR})
  target_include_directories(mockup PUBLIC ${R65816_INCLUDE_DIR})
  target_include_directories(mockup PUBLIC ${MOCKUPLIB_INCLUDE_DIR})

  target_link_libraries(mockup r65816)
  target_link_libraries(mockup libmockup)

  qt5_use_modules(mockup Widgets)
endif()
