#include "resources.h"

#include <fstream>
#include <cstdio>

static std::map<ResourceType, ResourceMap> resources;
static std::map<std::string, ResourceType> resource_type_table;

void Resources_Init()
{
	std::map<std::string, ResourceType>::iterator rttitr;

	resource_type_table["image"] = RT_IMAGE;
	resource_type_table["sound"] = RT_SOUND;
	resource_type_table["map"] = RT_MAP;
	resource_type_table["font"] = RT_FONT;
	resource_type_table["world"] = RT_WORLD;
	resource_type_table["animation"] = RT_ANIMATION;
	
	resources[RT_IMAGE] = ResourceMap();
	resources[RT_SOUND] = ResourceMap();
	resources[RT_MAP] = ResourceMap();
	resources[RT_FONT] = ResourceMap();
	resources[RT_WORLD] = ResourceMap();
	resources[RT_ANIMATION] = ResourceMap();

	std::ifstream file;
	file.open(RESOURCES_FILE);

	if ( !file.fail() )
	{
		char id[256], type[256], filename[FILENAME_MAX], line[FILENAME_MAX + 1024];
		while ( !file.eof() )
		{
			file.getline(line, sizeof(line));
			sscanf(line, "%s %s %[^\n]", id, type, filename);

			if ( (rttitr = resource_type_table.find(std::string(type))) != resource_type_table.end() )
			{
				ResourceType rt = rttitr->second;
				resources[rt][std::string(id)] = std::string(filename);
			}
		}
	}
}

std::string& Resources_GetFileName(ResourceType type, std::string name)
{
	return resources[type][name];
}

ResourceMap Resources_GetOfType(ResourceType type)
{
	return resources[type];
}
