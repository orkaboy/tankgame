#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class Camera
{
	public:
		Camera();
		
		void PositionCorner(vec2 tankPos, vec2 mouse, vec2 worldSize);
		vec2 GetCorner(void) { return corner; }
	private:
		vec2 corner;
		vec2 screenSize;
};

#endif
