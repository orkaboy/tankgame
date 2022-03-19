#pragma once

#include "gamestate.h"
#include "world.h"

namespace MDF {

class Game : public GameState {
public:
    void EnterState() override;
    void ExitState() override;
    
    std::string HandleEvents() override;
    std::string Update(float dt) override;
    void Draw() override;
private:
    World *mWorld{};
};

}
