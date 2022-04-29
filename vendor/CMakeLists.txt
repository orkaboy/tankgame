include(FetchContent)


FetchContent_Declare(freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG master
)
FetchContent_MakeAvailable(freetype)

add_library(Freetype::Freetype ALIAS freetype)

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

FetchContent_Declare(SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(SDL2_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SDL2_ttf)

# target_include_directories(SDL2_ttf PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/sdl2_ttf-src>)
# add_library(SDL2::ttf ALIAS SDL2_ttf)

FetchContent_Declare(SDL2_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SDL2_mixer)

FetchContent_Declare(SDL2_gfx
  GIT_REPOSITORY https://github.com/svn2github/sdl2_gfx.git
  GIT_TAG master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)
FetchContent_MakeAvailable(SDL2_gfx)
set(SDL2_gfx_SOURCE_DIR $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/sdl2_gfx-src>)

message(STATUS "GFX path: ${SDL2_gfx_SOURCE_DIR}")

add_library(SDL2_gfx
  ${SDL2_gfx_SOURCE_DIR}/SDL2_framerate.c
  ${SDL2_gfx_SOURCE_DIR}/SDL2_gfxPrimitives.c
  ${SDL2_gfx_SOURCE_DIR}/SDL2_imageFilter.c
  ${SDL2_gfx_SOURCE_DIR}/SDL2_rotozoom.c
)
target_include_directories(SDL2_gfx PUBLIC ${SDL2_gfx_SOURCE_DIR})
target_link_libraries(SDL2_gfx PUBLIC SDL2::SDL2)
add_library(SDL2::GFX ALIAS SDL2_gfx)
