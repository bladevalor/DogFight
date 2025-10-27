cmake_minimum_required(VERSION 3.28)
project(PocketSFML LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(pocket
    src/main.cpp

    src/Engine/Pickup.cpp
    src/Engine/Projectile.cpp
    src/Engine/Utility.cpp
    src/Engine/World.cpp
    src/Engine/SceneNode.cpp
    src/Game/Aircraft.cpp
    src/Game/DataTables.cpp
    src/Game/State.cpp
    src/Game/StateStack.cpp
)

target_include_directories(pocket PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_compile_features(pocket PRIVATE cxx_std_11)
target_link_libraries(pocket PRIVATE SFML::Graphics)
target_link_libraries(pocket PRIVATE SFML::Window)
target_link_libraries(pocket PRIVATE SFML::System)
