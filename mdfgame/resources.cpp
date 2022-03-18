#include "resources.h"

#include <fstream>
#include <cstdio>
#include <fmt/core.h>

namespace MDF {
namespace Resource {

static std::map<Type, ResourceMap> resources;
static std::map<std::string, Type> resource_type_table;

static std::string ResourceFilename(std::string resource_path, std::string filename) {
	return resource_path.empty() ? filename : fmt::format("{}/{}", resource_path, filename);
}

void Init(std::string resource_path)
{
	std::map<std::string, Type>::iterator rttitr;

	resource_type_table["image"] = Type::IMAGE;
	resource_type_table["sound"] = Type::SOUND;
	resource_type_table["map"] = Type::MAP;
	resource_type_table["font"] = Type::FONT;
	resource_type_table["world"] = Type::WORLD;
	resource_type_table["animation"] = Type::ANIMATION;
	
	resources[Type::IMAGE] = ResourceMap();
	resources[Type::SOUND] = ResourceMap();
	resources[Type::MAP] = ResourceMap();
	resources[Type::FONT] = ResourceMap();
	resources[Type::WORLD] = ResourceMap();
	resources[Type::ANIMATION] = ResourceMap();

	std::ifstream file;
	file.open(ResourceFilename(resource_path, RESOURCES_FILE));

	if ( !file.fail() )
	{
		char id[256], type[256], filename[FILENAME_MAX], line[FILENAME_MAX + 1024];
		while ( !file.eof() )
		{
			file.getline(line, sizeof(line));
			sscanf(line, "%s %s %[^\n]", id, type, filename);

			if ( (rttitr = resource_type_table.find(std::string(type))) != resource_type_table.end() )
			{
				Type rt = rttitr->second;
				resources[rt][std::string(id)] = ResourceFilename(resource_path, filename);
			}
		}
	}
}

std::string& GetFileName(Type type, std::string name)
{
	return resources[type][name];
}

ResourceMap GetOfType(Type type)
{
	return resources[type];
}

}
}
