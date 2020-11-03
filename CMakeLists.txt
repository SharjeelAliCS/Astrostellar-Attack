cmake_minimum_required(VERSION 2.6)

# Name of project
set(PROJ_NAME COMP3501_Project)
project(${PROJ_NAME} C CXX)

include_directories(${PROJECT_SOURCE_DIR}/assets)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include/node)
include_directories(${PROJECT_SOURCE_DIR}/src/node)
include_directories(${PROJECT_SOURCE_DIR}/include/node/entity)
include_directories(${PROJECT_SOURCE_DIR}/include/node/screen)
include_directories(${PROJECT_SOURCE_DIR}/include/node/entity/enemy)
include_directories(${PROJECT_SOURCE_DIR}/include/node/entity/obstacle)
include_directories(${PROJECT_SOURCE_DIR}/include/node/entity/player)

include_directories(${PROJECT_SOURCE_DIR}/src/node/entity)
include_directories(${PROJECT_SOURCE_DIR}/src/node/screen)
include_directories(${PROJECT_SOURCE_DIR}/src/node/entity/enemy)
include_directories(${PROJECT_SOURCE_DIR}/src/node/entity/obstacle)
include_directories(${PROJECT_SOURCE_DIR}/src/node/entity/player)
include_directories(${PROJECT_SOURCE_DIR}/include/world)
include_directories(${PROJECT_SOURCE_DIR}/src/world)
include_directories(${PROJECT_SOURCE_DIR}/include/setup)
include_directories(${PROJECT_SOURCE_DIR}/src/setup)
include_directories(${PROJECT_SOURCE_DIR}/include/attribute)
include_directories(${PROJECT_SOURCE_DIR}/src/attribute)
include_directories(${PROJECT_SOURCE_DIR}/include/menu)
include_directories(${PROJECT_SOURCE_DIR}/src/menu)

include_directories(${PROJECT_SOURCE_DIR}/resource)
#Used the following resource to do this:
#https://stackoverflow.com/questions/33808087/cmake-how-to-create-visual-studio-filters

# Specify project files: header files and source files
# Specify project source files
set(HDRS
    ${PROJECT_SOURCE_DIR}/include/attribute/orientation.h
    ${PROJECT_SOURCE_DIR}/include/attribute/sound.h

    ${PROJECT_SOURCE_DIR}/include/node/entity/entity_node.h

    ${PROJECT_SOURCE_DIR}/include/node/entity/enemy/enemy_node.h
    ${PROJECT_SOURCE_DIR}/include/node/entity/enemy/boss_node.h

    ${PROJECT_SOURCE_DIR}/include/node/entity/obstacle/obstacle_node.h
    ${PROJECT_SOURCE_DIR}/include/node/entity/obstacle/comet_node.h
    ${PROJECT_SOURCE_DIR}/include/node/entity/obstacle/asteroid_node.h
    ${PROJECT_SOURCE_DIR}/include/node/entity/obstacle/satellite_node.h
    ${PROJECT_SOURCE_DIR}/include/node/entity/obstacle/sun_node.h

    ${PROJECT_SOURCE_DIR}/include/node/entity/player/player_node.h

    ${PROJECT_SOURCE_DIR}/include/node/entity/projectile_node.h

    ${PROJECT_SOURCE_DIR}/include/node/screen/hud_node.h
    ${PROJECT_SOURCE_DIR}/include/node/screen/text_node.h

    ${PROJECT_SOURCE_DIR}/include/node/scene_node.h
    ${PROJECT_SOURCE_DIR}/include/node/skybox_node.h

    ${PROJECT_SOURCE_DIR}/include/setup/model_loader.h
    ${PROJECT_SOURCE_DIR}/include/setup/path_config.h
    ${PROJECT_SOURCE_DIR}/include/setup/resource.h
    ${PROJECT_SOURCE_DIR}/include/setup/resource_manager.h

    ${PROJECT_SOURCE_DIR}/include/world/camera.h
    ${PROJECT_SOURCE_DIR}/include/world/game.h
    ${PROJECT_SOURCE_DIR}/include/world/scene_graph.h

    ${PROJECT_SOURCE_DIR}/include/menu/menu.h
)
set(SRCS
    #SOURCE FILES
    ${PROJECT_SOURCE_DIR}/src/attribute/orientation.cpp
    ${PROJECT_SOURCE_DIR}/src/attribute/sound.cpp

    ${PROJECT_SOURCE_DIR}/src/node/entity/entity_node.cpp

    ${PROJECT_SOURCE_DIR}/src/node/entity/enemy/enemy_node.cpp
    ${PROJECT_SOURCE_DIR}/src/node/entity/enemy/boss_node.cpp

    ${PROJECT_SOURCE_DIR}/src/node/entity/obstacle/obstacle_node.cpp
    ${PROJECT_SOURCE_DIR}/src/node/entity/obstacle/comet_node.cpp
    ${PROJECT_SOURCE_DIR}/src/node/entity/obstacle/asteroid_node.cpp
    ${PROJECT_SOURCE_DIR}/src/node/entity/obstacle/satellite_node.cpp
    ${PROJECT_SOURCE_DIR}/src/node/entity/obstacle/sun_node.cpp

    ${PROJECT_SOURCE_DIR}/src/node/entity/player/player_node.cpp

    ${PROJECT_SOURCE_DIR}/src/node/entity/projectile_node.cpp

    ${PROJECT_SOURCE_DIR}/src/node/screen/hud_node.cpp
    ${PROJECT_SOURCE_DIR}/src/node/screen/text_node.cpp

    ${PROJECT_SOURCE_DIR}/src/node/scene_node.cpp
    ${PROJECT_SOURCE_DIR}/src/node/skybox_node.cpp

    ${PROJECT_SOURCE_DIR}/src/setup/resource.cpp
    ${PROJECT_SOURCE_DIR}/src/setup/resource_manager.cpp

    ${PROJECT_SOURCE_DIR}/src/world/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/world/game.cpp
    ${PROJECT_SOURCE_DIR}/src/world/scene_graph.cpp

    ${PROJECT_SOURCE_DIR}/src/menu/menu.cpp

    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

set(RCRS
    ${PROJECT_SOURCE_DIR}/resource/material_fp.glsl
    ${PROJECT_SOURCE_DIR}/resource/material_vp.glsl
    ${PROJECT_SOURCE_DIR}/resource/procedural_fp.glsl
    ${PROJECT_SOURCE_DIR}/resource/procedural_vp.glsl
    ${PROJECT_SOURCE_DIR}/resource/textured_material_fp.glsl
    ${PROJECT_SOURCE_DIR}/resource/textured_material_vp.glsl
    ${PROJECT_SOURCE_DIR}/resource/screen_fp.glsl
    ${PROJECT_SOURCE_DIR}/resource/screen_vp.glsl
)
#https://stackoverflow.com/questions/31422680/how-to-set-visual-studio-filters-for-nested-sub-directory-using-cmake
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RCRS})
##source_group("Header Files\\node" ${HEADER_FILES_NODE})
#source_group("Header Files\\world" ${HEADER_FILES_WORLD})
#source_group("Header Files\\setup" ${HEADER_FILES_SETUP})
#source_group("Header Files\\attribute" ${HEADER_FILES_ATTRIBUTE})

#source_group("Source Files\\node" ${SOURCE_FILES_NODE})
#source_group("Source Files\\world" ${SOURCE_FILES_WORLD})
#source_group("Source Files\\setup" ${SOURCE_FILES_SETUP})
#source_group("Source Files\\attribute" ${SOURCE_FILES_ATTRIBUTE})

set(ASSETS
   ${PROJECT_SOURCE_DIR}/assets
)

file(COPY ASSETS DESTINATION ${CMAKE_BINARY_DIR})

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS} ${RCRS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

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
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})


# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)

    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
