cmake_minimum_required(VERSION 3.16)
project(VirtualSupermarket LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use an installed SFML 3 if there is one, otherwise download and build it.
find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
if(NOT SFML_FOUND)
    include(FetchContent)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON)
    FetchContent_MakeAvailable(SFML)
endif()

file(GLOB GAME_SOURCES CONFIGURE_DEPENDS MyGame/src/*.cpp)
add_executable(VirtualSupermarket MyGame/main.cpp ${GAME_SOURCES})
target_include_directories(VirtualSupermarket PRIVATE MyGame)
target_link_libraries(VirtualSupermarket PRIVATE SFML::Graphics)

if(MSVC)
    target_compile_options(VirtualSupermarket PRIVATE /W4)
else()
    target_compile_options(VirtualSupermarket PRIVATE -Wall -Wextra)
endif()

# The game reads Images/, fonts/ and Products.txt from its working directory,
# so copy them next to the executable after every build.
add_custom_command(TARGET VirtualSupermarket POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/MyGame/Images $<TARGET_FILE_DIR:VirtualSupermarket>/Images
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/MyGame/fonts $<TARGET_FILE_DIR:VirtualSupermarket>/fonts
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/MyGame/Products.txt $<TARGET_FILE_DIR:VirtualSupermarket>/Products.txt)

# Tests for the logic that does not need a window:  ctest --test-dir build
include(CTest)
if(BUILD_TESTING)
    add_executable(GameTests tests/tests.cpp
        MyGame/src/Cart.cpp MyGame/src/Money.cpp MyGame/src/Payment.cpp
        MyGame/src/Product.cpp MyGame/src/ProductCatalog.cpp)
    target_include_directories(GameTests PRIVATE MyGame/src)
    add_test(NAME GameTests COMMAND GameTests)
endif()
