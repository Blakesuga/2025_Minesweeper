cmake_minimum_required(VERSION 3.28)

project(Minesweeper_Spring_2024 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# path to SFML 2.6
set(SFML_DIR "C:/Users/pnata/CLionProjects/Minesweeper_Spring_2024/SFML-2.6.1/lib/cmake/SFML")

# find SFML 2.6 components
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

# add source files
add_executable(main
        src/main.cpp
        src/Tile.cpp
        src/Helpers.cpp
        src/WelcomeWindow.cpp
        src/Leaderboard.cpp
        src/GameWindow.cpp
        src/Timer.cpp
)

# link SFML targets
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system)

# copy SFML DLLs
add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/Users/pnata/CLionProjects/Minesweeper_Spring_2024/SFML-2.6.1/bin"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# copy MinGW DLLs
if(MINGW)
    get_filename_component(MINGW_BIN_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
    file(GLOB MINGW_DLLS "${MINGW_BIN_PATH}/*.dll")
    file(COPY ${MINGW_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# copy assets
add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/files
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/files
)