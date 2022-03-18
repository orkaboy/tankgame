#pragma once

#include <map>
#include <string>

namespace MDF {
namespace Resource {

constexpr const char* RESOURCES_FILE = "resources.mdf";

enum class Type
{
	IMAGE,
	SOUND,
	MAP,
	FONT,
	ANIMATION,
	WORLD
};

typedef std::map<std::string, std::string> ResourceMap;

void Init(std::string resource_path);

std::string& GetFileName(Type type, std::string name);
ResourceMap GetOfType(Type type);

}
}
