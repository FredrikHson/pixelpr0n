cmake_minimum_required(VERSION 3.3)

project(pixelpr0n)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})
add_definitions(${SDL2_DEFINITIONS})

set(COMMON_SRC
    "src/main.cpp"
    "src/fire.cpp"
    "src/liquid.cpp"
    "src/sand.cpp"
    "src/burningsand.cpp"
    "src/fpscounter.cpp"
    "src/drawshapes.cpp"
    "src/matrix.cpp"
    "src/vec4.cpp"
    "src/mesh.cpp"
    "src/voronoi.cpp"
    "externals/fast_obj_loader/fast_obj_loader.cpp"
    )

include_directories(
    "src/"
    "externals/fast_obj_loader/"
    ${SDL_INCLUDE_DIR}
    )


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SRC})
message("${SDL_LIBRARY}")
target_link_libraries(${CMAKE_PROJECT_NAME}
    ${SDL2_LIBRARIES}
    )

find_program(iwyu_path NAMES include-what-you-use iwyu)
if(NOT iwyu_path)
    message(WARNING "Could not find the program include-what-you-use")
else()
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY C_INCLUDE_WHAT_YOU_USE ${iwyu_path})
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
