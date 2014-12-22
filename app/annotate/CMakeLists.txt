set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(libxml2_DIR .)
#find_package(libxml2)
#include_directories(${LIBXML2_INCLUDE_DIR})
#message( STATUS ${LIBXML2_LIBRARIES})
add_executable(annotate
  base64.c
  main.c
  )

add_executable(annotate_tracer
  annotate_tracer.c
  )

target_link_libraries(annotate
  ${LIBXML2_LIBRARIES}
  )
