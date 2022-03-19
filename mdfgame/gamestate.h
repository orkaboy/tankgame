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
    virtual std::string Update(float dt) { return ""; }
    virtual void Draw() {}
};

class GameSM {
public:
    ~GameSM();

    void Run();

    void RegisterState(std::string name, GameState* state) {
        mStates[name] = state;
    }
    void TransitionState(std::string name);
private:
    std::map<std::string, GameState*> mStates;

    GameState* mCurState{};

    bool mQuit{false};
};

}
