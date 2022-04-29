#include "gamestate.h"
#include "graphics.h"
#include <fmt/core.h>

namespace MDF {

GameSM::~GameSM() {
    for(auto state : mStates) {
        delete state.second;
    }
}

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

        while (accumulator >= dt) {
            accumulator -= dt;
            /* Handle event loop */
            auto stateName = mCurState->HandleEvents();
            TransitionState(stateName);

            /* Run Update based on deltatime in seconds */
            stateName = mCurState->Update(dt);
            TransitionState(stateName);
        }

        /* Draw scene */
        Graphics::BeginScene();
        mCurState->Draw();
        Graphics::EndScene();

        // TODO wait
        //SDL_Delay(10);
    }
}


void GameSM::TransitionState(std::string stateName) {
    if(stateName == "QUIT") {
        mQuit = true;
        fmt::print("Quitting...\n");
    }
    else if(!stateName.empty()) {
        auto newState = mStates[stateName];
        if(newState) {
            if(mCurState) {
                mCurState->ExitState();
            }
            mCurState = newState;
            mCurState->EnterState();
        }
    }
}

}
