cmake_minimum_required( VERSION 3.1 )

project( Vizior )

find_package( OpenGL REQUIRED )

set( CMAKE_CXX_STANDARD 17 )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
SET( GCC_COVERAGE_COMPILE_FLAGS "-Wwrite-strings" )

add_subdirectory( external/glfw )
   
add_executable( Vizior WIN32 src/main.cpp src/glad.c src/Point2D.cpp src/ImageBuilder.cpp src/Window.cpp src/Vizior.cpp )
target_link_libraries( Vizior ${OPENGL_LIBRARIES} glfw )
target_include_directories( Vizior PUBLIC ./include PUBLIC external/glfw/include PUBLIC external )
