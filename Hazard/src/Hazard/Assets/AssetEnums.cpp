
#include <hzrpch.h>
#include "AssetEnums.h"
#include "Asset.h"

namespace Hazard::Utils
{

	AssetType StringToAssetType(const std::string& type)
	{
		if (type == "None")					return AssetType::Undefined;
		if (type == "AudioClip")			return AssetType::AudioClip;
		if (type == "World")				return AssetType::World;
		if (type == "Image")				return AssetType::Image;
		if (type == "PhysicsMaterial")		return AssetType::PhysicsMaterial;
		if (type == "Mesh")					return AssetType::Mesh;
		if (type == "EnvironmentMap")		return AssetType::EnvironmentMap;
		if (type == "Script")				return AssetType::Script;
		if (type == "Pipeline")				return AssetType::Pipeline;
		if (type == "Shader")				return AssetType::Shader;
		if (type == "Material")             return AssetType::Material;
		if (type == "Buffer")				return AssetType::Buffer;

		return AssetType::Undefined;
	}
	const char* AssetTypeToString(AssetType type)
	{
		switch (type)
		{
			case AssetType::Undefined:			return "Undefined";
			case AssetType::AudioClip:			return "AudioClip";
			case AssetType::World:				return "World";
			case AssetType::Image:				return "Image";
			case AssetType::Mesh:				return "Mesh";
			case AssetType::PhysicsMaterial:	return "PhysicsMaterial";
			case AssetType::EnvironmentMap:		return "EnvironmentMap";
			case AssetType::Script:				return "Script";
			case AssetType::Pipeline:			return "Pipeline";
			case AssetType::Shader:				return "Shader";
			case AssetType::Material:           return "Material";
			case AssetType::Buffer:				return "Buffer";
			default:							return "Unknown";
		}
		return "";
	}
	AssetType AssetTypeFromExtension(const std::string& ext)
	{
		if (ext == ".jpeg")		return AssetType::Image;
		if (ext == ".jpg")		return AssetType::Image;
		if (ext == ".png")		return AssetType::Image;
		if (ext == ".hdr")		return AssetType::EnvironmentMap;
		if (ext == ".tga")		return AssetType::Image;
		if (ext == ".mp3")		return AssetType::AudioClip;
		if (ext == ".ogg")		return AssetType::AudioClip;
		if (ext == ".hazard")	return AssetType::World;
		if (ext == ".obj")		return AssetType::Mesh;
		if (ext == ".fbx")		return AssetType::Mesh;
		if (ext == ".dae")		return AssetType::Mesh;
		if (ext == ".gltf")		return AssetType::Mesh;
		if (ext == ".glb")		return AssetType::Mesh;
		if (ext == ".cs")		return AssetType::Script;
		if (ext == ".glsl")		return AssetType::Shader;
		if (ext == ".hmat")     return AssetType::Material;

		return AssetType::Undefined;
	}
}
using namespace Hazard;

template<>
bool YamlUtils::Deserialize(YAML::Node node, const std::string& key, AssetType& value, AssetType defaultValue)
{
	if (!node[key])
	{
		value = defaultValue;
		return false;
	}

	value = Utils::StringToAssetType(node[key].as<std::string>());
	return true;
}
template<>
void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, AssetType value)
{
	out << YAML::Key << key << YAML::Value << Utils::AssetTypeToString(value);
}
template<>
bool YamlUtils::Deserialize(YAML::Node node, const std::string& key, AssetHandle& value, AssetHandle defaultValue)
{
	if (!node[key])
	{
		value = defaultValue;
		return false;
	}
	value = node[key].as<uint64_t>();
	return true;
}
template<>
void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, AssetHandle value)
{
	out << YAML::Key << key << YAML::Value << (uint64_t)value;
}
