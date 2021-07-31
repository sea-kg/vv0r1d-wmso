cmake_minimum_required(VERSION 3.0)

project(vv-server C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${vv-server_SOURCE_DIR})

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/api")
list (APPEND WSJCPP_INCLUDE_DIRS "src/game")

list (APPEND WSJCPP_SOURCES "src/main.cpp")

#### BEGIN_WSJCPP_APPEND
#### END_WSJCPP_APPEND

include_directories(${WSJCPP_INCLUDE_DIRS})

# api
list (APPEND WSJCPP_SOURCES "./src/api/light_web_http_handler_api.cpp")

# game
list (APPEND WSJCPP_SOURCES "./src/game/game_map_objects.cpp")

add_executable (vv-server ${WSJCPP_SOURCES})

target_link_libraries(vv-server ${WSJCPP_LIBRARIES})

install(
    TARGETS
        vv-server
    RUNTIME DESTINATION
        /usr/bin
)

