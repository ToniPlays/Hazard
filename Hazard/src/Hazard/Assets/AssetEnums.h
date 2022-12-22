#pragma once

#include <string>
#include "Utility/YamlUtils.h"

using AssetHandle = UID;

enum class AssetType : uint32_t
{
	Undefined = 0,
	Folder,
	Image,
	AudioClip,
	World,
	EnvironmentMap,
	PhysicsMaterial,
	Mesh,
	Script,
	Pipeline,
	Shader
};

enum class ResourceType 
{
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
void YamlUtils::Deserialize(YAML::Node node, const std::string& key, AssetType& value, AssetType defaultValue);
template<>
void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, AssetType value);
template<>
void YamlUtils::Deserialize(YAML::Node node, const std::string& key, AssetHandle& value, AssetHandle defaultValue);
template<>
void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, AssetHandle value);
