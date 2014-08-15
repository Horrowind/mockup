set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt5Widgets REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/bsnes")

#link_directories("${CMAKE_SOURCE_DIR}/bsnes/obj")

#add_library(processor-r65816 OBJECT IMPORTED)
set_property(TARGET bsnes-r65816 PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bsnes/obj/processor-r65816.o)

ADD_EXECUTABLE(mockup
  main.cpp
  main_window.cpp
  testprocessor.cpp
)
target_link_libraries(mockup ${CMAKE_SOURCE_DIR}/bsnes/obj/processor-r65816.o)

qt5_use_modules(mockup Widgets)
