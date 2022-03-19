#include "camera.h"
#include "world.h"

Camera::Camera() : corner(0,0), screenSize(SCREEN_WIDTH, SCREEN_HEIGHT)
{
	
}

void Camera::PositionCorner(vec2 tankPos, vec2 mouse)
{
	vec2 modTankPos = vec2Multiply(tankPos, 4);
	vec2 newCorner = vec2Add(modTankPos, mouse);
	newCorner = vec2Multiply(newCorner, 0.2);
	vec2 screenMidpoint = vec2Multiply(screenSize, 0.5);
	newCorner = vec2Sub(newCorner, screenMidpoint);

// 	if(newCorner.x < 0)
// 		newCorner.x = 0;
// 	else if(newCorner.x > worldSize.width - screenSize.width)
// 		newCorner.x = worldSize.width - screenSize.width;
// 	if(newCorner.y < 0)
// 		newCorner.y = 0;
// 	else if(newCorner.y > worldSize.height - screenSize.height)
// 		newCorner.y = worldSize.height - screenSize.height;
	
	corner = newCorner;
}
