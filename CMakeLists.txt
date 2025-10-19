cmake_minimum_required( VERSION 3.21 )

project( Vizior )

find_package( OpenGL REQUIRED )

set( CMAKE_CXX_STANDARD 17 )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
SET( GCC_COVERAGE_COMPILE_FLAGS "-Wwrite-strings" )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

add_subdirectory( external/glfw )
add_subdirectory( external/freetype )
   
add_library( Vizior STATIC )
target_sources(Vizior 
  PUBLIC
    ./include/Point2D.hpp 
    ./include/ImageBuilder.hpp 
    ./include/Window.hpp 
    ./include/Texture.hpp 
    ./include/FontManager.hpp 
    ./include/Shaders.hpp 
    ./include/Camera.hpp 
    ./include/Vizior.hpp
  PRIVATE
    ./src/glad.c 
    ./src/Point2D.cpp 
    ./src/ImageBuilder.cpp 
    ./src/Window.cpp 
    ./src/stbImageLoader.cpp 
    ./src/Texture.cpp 
    ./src/FontManager.cpp 
    ./src/Shaders.cpp 
    ./src/Camera.cpp 
    ./src/Vizior.cpp)

target_include_directories( Vizior PUBLIC ./include PUBLIC external/glfw/include PUBLIC external/FreeType/include PUBLIC external )
target_link_libraries( Vizior ${OPENGL_LIBRARIES} glfw freetype )

add_executable( vzrSample WIN32 src/main.cpp)
target_include_directories(vzrSample PUBLIC ./include)
target_link_libraries(vzrSample Vizior)
