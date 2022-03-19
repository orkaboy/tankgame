#include "gamestate.h"
#include "graphics.h"

namespace MDF {

void GameState::Run() {
    const float dt = 1.0 / FPS;
    float currentTime = SDL_GetTicks();
    float accumulator = 0.0f;

    while(!mQuit) {
        // Update time
        float newTime = (float)SDL_GetTicks();
        float deltaTime = (newTime - currentTime) / 1000.f;
        currentTime = newTime;
        accumulator += deltaTime;

        while (accumulator >= dt)
        {
            accumulator -= dt;

            HandleEvents();

            Update(dt);
        }

        Graphics::BeginScene();
        Draw();
        Graphics::EndScene();

        // TODO wait
    }
}

}
