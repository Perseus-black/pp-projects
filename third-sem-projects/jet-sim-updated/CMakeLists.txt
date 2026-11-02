cmake_minimum_required(VERSION 3.20)
project(JetSim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

set(SOURCES
    src/main.cpp
    src/MathUtils.cpp
    src/Aircraft.cpp
    src/Contact.cpp
    src/Radar.cpp
    src/HUD.cpp
    src/AudioVisual.cpp
    src/Weapons.cpp
)

set(HEADERS
    include/Config.hpp
    include/MathUtils.hpp
    include/Aircraft.hpp
    include/Contact.hpp
    include/Radar.hpp
    include/HUD.hpp
    include/AudioVisual.hpp
    include/Weapons.hpp
)

add_executable(jet-sim-modern ${SOURCES} ${HEADERS})

target_include_directories(jet-sim-modern PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(jet-sim-modern PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

add_custom_command(TARGET jet-sim-modern POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/f22.png"
    "$<TARGET_FILE_DIR:jet-sim-modern>/f22.png"
)
