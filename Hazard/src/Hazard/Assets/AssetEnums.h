#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard 
{
	enum class AssetType
	{
		Undefined = 0,
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

	namespace Utils 
	{
		AssetType StringToAssetType(const std::string& type);
		const char* AssetTypeToString(AssetType type);
		AssetType AssetTypeFromExtension(const std::string& ext);

		const char* ResourceTypeToString(ResourceType type);
	}
} 