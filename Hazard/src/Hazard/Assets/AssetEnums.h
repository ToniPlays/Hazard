#pragma once

#include <string>
#include "Utility/YamlUtils.h"

using AssetHandle = UID;

enum class AssetType
{
	Undefined = 0,
	Folder,
	AudioClip,
	World,
	Image,
	Mesh,
	Script,
	EnvironmentMap,
	PhysicsMaterial
};

enum class ResourceType {
	Undefined = 0,
	VertexBuffer,
	IndexBuffer,
	UniformBuffer,
	Pipeline,
	Mesh
};

namespace Hazard::Utils
{
	AssetType StringToAssetType(const std::string& type);
	const char* AssetTypeToString(AssetType type);
	AssetType AssetTypeFromExtension(const std::string& ext);

	const char* ResourceTypeToString(ResourceType type);
}
template<>
static void YamlUtils::Deserialize(YAML::Node node, const std::string& key, AssetType& value, AssetType defaultValue);
template<>
static void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, AssetType value);
template<>
static void YamlUtils::Deserialize(YAML::Node node, const std::string& key, AssetHandle& value, AssetHandle defaultValue);
template<>
static void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, AssetHandle value);