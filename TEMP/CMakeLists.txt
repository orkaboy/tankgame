cmake_minimum_required(VERSION 3.14)

project(sdl-test VERSION 0.1)

include(FetchContent)

FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 8.1.1
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SDL2)


add_executable(sdl-test main.cpp)
target_link_libraries(sdl-test SDL2::SDL2 fmt::fmt)
