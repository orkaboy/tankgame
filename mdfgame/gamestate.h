#pragma once

namespace MDF {

constexpr int FPS = 30;

class GameState {
public:
    void Run();

protected:
    virtual void HandleEvents() = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;

private:
    bool mQuit{false};
};

}
