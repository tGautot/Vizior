cmake_minimum_required( VERSION 3.21 )

project( Vizior )

find_package( OpenGL REQUIRED )

set( CMAKE_CXX_STANDARD 17 )

include_directories( ${OPENGL_INCLUDE_DIRS} )

SET( GCC_COVERAGE_COMPILE_FLAGS "-Wwrite-strings" )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL ON )
add_subdirectory( external/glfw )
add_subdirectory( external/freetype )

set(exported_headers 
    ./include/Common.hpp
    ./include/Point2D.hpp 
    ./include/ImageBuilder.hpp 
    ./include/Window.hpp 
    ./include/Texture.hpp 
    ./include/FontManager.hpp 
    ./include/Shaders.hpp 
    ./include/Camera.hpp 
    ./include/Vizior.hpp

    )
   
add_library( Vizior STATIC )
target_compile_options(Vizior PUBLIC -g)
target_sources(Vizior 
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




target_include_directories( Vizior 
  PUBLIC 
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Vizior>"
  PRIVATE 
  ./external/glfw/include 
  ./external/FreeType/include 
  ./external )

include(GNUInstallDirs)
target_link_libraries( Vizior PUBLIC ${OPENGL_LIBRARIES} PUBLIC glfw freetype )


# Sample executable
add_executable( vzrSample WIN32 src/main.cpp)
target_include_directories(vzrSample PUBLIC ./include)
target_link_libraries(vzrSample Vizior)


# Exportation for easy usage in other projects


install(TARGETS Vizior
        EXPORT ViziorTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${exported_headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Vizior)
install(FILES ./include/glad/glad.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Vizior/glad)
install(FILES ./include/KHR/khrplatform.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Vizior/KHR)


message(CMAKE_INSTALL_LIBDIR="${CMAKE_INSTALL_LIBDIR}")

install(EXPORT ViziorTargets
        FILE ViziorTargets.cmake
        NAMESPACE Vizior::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vizior
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/ViziorConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vizior
)

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/ViziorConfig.cmake"
        #  "${CMAKE_CURRENT_BINARY_DIR}/ViziorConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vizior
)