#include "gamestate.h"
#include "graphics.h"

namespace MDF {

void GameSM::Run() {
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
            /* Handle event loop */
            auto stateName = mCurState->HandleEvents();

            /* Handle State transition */
            if(!stateName.empty()) {
                auto newState = mStates[stateName];
                if(newState) {
                    TransitionState(newState);
                    break;
                }
            }

            /* Run Update based on deltatime in seconds */
            mCurState->Update(dt);
        }

        /* Draw scene */
        Graphics::BeginScene();
        mCurState->Draw();
        Graphics::EndScene();

        // TODO wait
    }
}


void GameSM::TransitionState(GameState* newState) {
    if(mCurState) {
        mCurState->ExitState();
    }
    mCurState = newState;
    mCurState->EnterState();
}

}
