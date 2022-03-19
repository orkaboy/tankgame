#pragma once

#include <map>
#include <string>

namespace MDF {

constexpr int FPS = 30;

class GameState {
public:
    virtual void EnterState() {}
    virtual void ExitState() {}

    virtual std::string HandleEvents() { return ""; }
    virtual void Update(float dt) {}
    virtual void Draw() {}
};

class GameSM {
public:
    void Run();

    void RegisterState(std::string name, GameState* state) {
        mStates[name] = state;
    }
    void TransitionState(GameState* newState);
private:
    std::map<std::string, GameState*> mStates;

    GameState* mCurState{};

    bool mQuit{false};
};

}
