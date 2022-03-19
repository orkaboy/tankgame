#pragma once

#include "utility.h"

namespace MDF {

class Camera
{
	public:
		Camera();
		
		void PositionCorner(vec2 tankPos, vec2 mouse);
		auto GetCorner() const -> vec2 { return corner; }
	private:
		vec2 corner;
		vec2 screenSize;
};

}
