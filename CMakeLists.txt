cmake_minimum_required(VERSION 3.20)
Include(FetchContent)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(DxChess)

# Common interface for compiler settings

add_library(common_settings INTERFACE)

if(MSVC)
  target_compile_options(common_settings INTERFACE /W4 /WX)
  target_link_options(common_settings INTERFACE /DEBUG)
else()
  target_compile_options(common_settings INTERFACE -Wall -Wextra -Werror)
endif()

add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=1>)

# Core DxChess library
add_library(dxchess)

target_sources(dxchess PRIVATE 
    src/DxChess/Bitboard.cpp
    src/DxChess/Coord.cpp
)
target_include_directories(dxchess PUBLIC inc/DxChess)
target_link_libraries(dxchess PRIVATE common_settings)


add_executable(chess)
target_sources(chess PRIVATE src/main.cpp)
target_link_libraries(chess PRIVATE common_settings dxchess)


# Tests

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests)

target_sources(tests PRIVATE 
    tests/Bitboard.cpp
    tests/Coord.cpp
)
set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries(tests PRIVATE common_settings)
target_link_libraries(tests PRIVATE dxchess)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
 