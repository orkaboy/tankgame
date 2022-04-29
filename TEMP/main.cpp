#include <fmt/core.h>
#include <SDL.h>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int SCREEN_BPP = 32;


auto main(int argc, char* argv[]) -> int {
    fmt::print("Hello world!\n");
    
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    fmt::print("Compiled against SDL version {}.{}.{}.\n",
        compiled.major, compiled.minor, compiled.patch);
    fmt::print("Linking against SDL version {}.{}.{}.\n",
        linked.major, linked.minor, linked.patch);
    
	// initialize SDL video
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fmt::print( "Unable to init SDL: {}\n", SDL_GetError() );
		return 0;
	}

    atexit(SDL_Quit);
	// create a new window
    auto screen = SDL_CreateWindow("SDL_Test",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              0,
                              0,
                              SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!screen)
	{
		fmt::print("Unable to set {}x{} video: {}\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		return 0;
	}

    auto screenSurface = SDL_GetWindowSurface(screen);

    auto renderer = SDL_CreateRenderer(screen, -1, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);


    bool quit = false;
    while(!quit) {
        
    SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                fmt::print("SDL_QUIT\n");
                quit = true;
            }

            if(event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE: // TODO Bug in SDL2 2.0.10 in WSL2
                //case SDL_SCANCODE_2:
                    fmt::print("SDL_SCANCODE_ESCAPE\n");
                    quit = true;
                }
            }
        }


        SDL_SetRenderDrawColor(renderer, 128, 64, 0, 255);
        SDL_RenderClear(renderer);




        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    return 0;
}
