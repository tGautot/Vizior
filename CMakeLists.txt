cmake_minimum_required( VERSION 3.1 )

project( Vizior )

find_package( OpenGL REQUIRED )

set( CMAKE_CXX_STANDARD 17 )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( external/glfw )
     
add_executable( vizior WIN32 src/main.cpp src/glad.c src/vizior.cpp )
target_link_libraries( vizior ${OPENGL_LIBRARIES} glfw )
target_include_directories( vizior PUBLIC ./include PUBLIC external/glfw/include PUBLIC external )
