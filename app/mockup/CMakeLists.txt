set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt5Widgets REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/bsnes")

ADD_EXECUTABLE(mockup
  main.cpp
  main_window.cpp
)

qt5_use_modules(mockup Widgets)
