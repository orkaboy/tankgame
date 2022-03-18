#pragma once

constexpr float GRAVITY = 6.67259;

#include "world.h"
#include "utility.h"

namespace MDF {
namespace Physics {

void Process(World &w, float dt);

}
}
