#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <string>

#define RESOURCES_FILE "resources.mdf"

enum ResourceType
{
	RT_IMAGE,
	RT_SOUND,
	RT_MAP,
	RT_FONT,
	RT_ANIMATION,
	RT_WORLD
};

typedef std::map<std::string, std::string> ResourceMap;

void Resources_Init();

std::string& Resources_GetFileName(ResourceType type, std::string name);
ResourceMap Resources_GetOfType(ResourceType type);

#endif
