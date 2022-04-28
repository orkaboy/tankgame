include(FetchContent)

find_package(Freetype REQUIRED)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.1.1
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.0.22
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-2.0.5
)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(SDL2_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG release-2.0.18
)
FetchContent_MakeAvailable(SDL2_ttf)

FetchContent_Declare(SDL2_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG release-2.0.4
)
FetchContent_MakeAvailable(SDL2_mixer)

FetchContent_Declare(SDL2_gfx
  GIT_REPOSITORY https://github.com/svn2github/sdl2_gfx.git
  GIT_TAG master
)
FetchContent_MakeAvailable(SDL2_gfx)
