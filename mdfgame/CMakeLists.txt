
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_mixer REQUIRED)


add_library(utility utility.cpp)
add_library(GAME::utility ALIAS utility)

add_library(resources resources.cpp)
target_link_libraries(resources fmt::fmt)
add_library(GAME::resources ALIAS resources)

add_library(audio audio.cpp)
target_link_libraries(audio GAME::resources SDL2::Mixer fmt::fmt)
add_library(GAME::audio ALIAS audio)


add_library(game animation.cpp button.cpp camera.cpp effects.cpp graphics.cpp input.cpp map.cpp menu.cpp physics.cpp planet.cpp player.cpp projectile.cpp tank.cpp weapon.cpp world.cpp)
target_link_libraries(game GAME::utility GAME::audio SDL2::Main SDL2::Image SDL2::TTF SDL2::GFX fmt::fmt)
add_library(GAME::lib ALIAS game)


add_executable(tank_game main.cpp)
target_link_libraries(tank_game GAME::lib)
