cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(DxChess)

file(GLOB SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_executable(chess)

target_sources(chess PRIVATE ${SRC})

if(MSVC)
  target_compile_options(chess PRIVATE /W4 /WX)
  target_link_options(chess PUBLIC /DEBUG)
else()
  target_compile_options(chess PRIVATE -Wall -Wextra -Werror -g -Wno-unused-parameter)
endif()