cmake_minimum_required(VERSION 2.6)

# Name of project
project(COMP3501_A3)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/resource)
# Specify project source files
set(SRCS
    
    ${PROJECT_SOURCE_DIR}/include/camera.h
    ${PROJECT_SOURCE_DIR}/include/game.h 
    ${PROJECT_SOURCE_DIR}/include/gameobj.h
    ${PROJECT_SOURCE_DIR}/include/transformation.h
    ${PROJECT_SOURCE_DIR}/include/projectileobj.h
    ${PROJECT_SOURCE_DIR}/include/playerobj.h
    ${PROJECT_SOURCE_DIR}/include/enemyobj.h
    ${PROJECT_SOURCE_DIR}/include/rotation.h
    ${PROJECT_SOURCE_DIR}/include/geometry.h
    ${PROJECT_SOURCE_DIR}/include/asteroid.h
    ${PROJECT_SOURCE_DIR}/include/resource.h
    ${PROJECT_SOURCE_DIR}/include/resource_manager.h
    ${PROJECT_SOURCE_DIR}/include/scene_graph.h
    ${PROJECT_SOURCE_DIR}/include/scene_node.h
    

    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/game.cpp
    ${PROJECT_SOURCE_DIR}/src/gameobj.cpp
    ${PROJECT_SOURCE_DIR}/src/transformation.cpp
    ${PROJECT_SOURCE_DIR}/src/projectileobj.cpp
    ${PROJECT_SOURCE_DIR}/src/playerobj.cpp
    ${PROJECT_SOURCE_DIR}/src/enemyobj.cpp
    ${PROJECT_SOURCE_DIR}/src/rotation.cpp
    ${PROJECT_SOURCE_DIR}/src/geometry.cpp
    ${PROJECT_SOURCE_DIR}/src/asteroid.cpp
    ${PROJECT_SOURCE_DIR}/src/resource.cpp
    ${PROJECT_SOURCE_DIR}/src/resource_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/scene_graph.cpp
    ${PROJECT_SOURCE_DIR}/src/scene_node.cpp
    
    ${PROJECT_SOURCE_DIR}/src/main.cpp

)

# Add executable based on the source files
add_executable(A3 ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(A3 ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH C:/Libraries)
include_directories(${LIBRARY_PATH}/include)

if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(A3 ${GLEW_LIBRARY})
target_link_libraries(A3 ${GLFW_LIBRARY})
target_link_libraries(A3 ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)

    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(A3 PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
