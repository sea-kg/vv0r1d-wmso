cmake_minimum_required(VERSION 3.0)

project(game-change-the-world C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${game-change-the-world_SOURCE_DIR})

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")

list (APPEND WSJCPP_SOURCES "src/main.cpp")

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (game-change-the-world ${WSJCPP_SOURCES})

target_link_libraries(game-change-the-world ${WSJCPP_LIBRARIES})

install(
    TARGETS
        game-change-the-world
    RUNTIME DESTINATION
        /usr/bin
)

