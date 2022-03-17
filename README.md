# Requirements

Install the following libraries in order to build the game:

```
sudo apt install g++ cmake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-gfx-dev libsdl2-mixer-dev
```

# Building

Build using CMake.

```
mkdir build
cd build
cmake ..
make -j
```

# Running

The game must be run from this directory (in order to access the data files). If built using cmake, run:

```
./build/mdfgame/tank_game
```
