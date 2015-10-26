CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
LD = g++
LDFLAGS = -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net

PROGRAMS = tank_game

all: ${PROGRAMS}

OBJS = planet.cpp physics.cpp graphics.cpp projectile.cpp main.cpp world.cpp player.cpp tank.cpp input.cpp audio.cpp resources.cpp utility.cpp net.cpp net_server.cpp net_client.cpp net_util.cpp weapon.cpp animation.cpp effects.cpp button.cpp camera.cpp menu.cpp

tank_game: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} ${LDFLAGS} -o $@

clean:
	rm -f ${PROGRAMS} *.o
