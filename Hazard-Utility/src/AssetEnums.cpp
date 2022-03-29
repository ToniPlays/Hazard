
#include "AssetEnums.h"

namespace HazardUtility {
	namespace Utils 
	{
		AssetType StringToAssetType(const std::string& type) {
			if (type == "None")					return AssetType::Undefined;
			if (type == "AudioClip")			return AssetType::AudioClip;
			if (type == "World")				return AssetType::World;
			if (type == "Texture")				return AssetType::Image;
			if (type == "PhysicsMaterial")		return AssetType::PhysicsMaterial;
			if (type == "Mesh")					return AssetType::Mesh;
			if (type == "EnvironmentMap")		return AssetType::EnvironmentMap;
			if (type == "Script")				return AssetType::Script;

			return AssetType::Undefined;
		}
		const char* AssetTypeToString(AssetType type) {
			switch (type)
			{
			case AssetType::Undefined:			return "None";
			case AssetType::AudioClip:			return "AudioClip";
			case AssetType::World:				return "World";
			case AssetType::Image:				return "Image";
			case AssetType::Mesh:				return "Mesh";
			case AssetType::PhysicsMaterial:	return "PhysicsMaterial";
			case AssetType::EnvironmentMap:		return "EnvironmentMap";
			case AssetType::Script:				return "Script";
			default:							return "Unknown";
			}
			return "";
		}
		AssetType AssetTypeFromExtension(const std::string& ext) {
			if (ext == "jpeg")		return AssetType::Image;
			if (ext == "jpg")		return AssetType::Image;
			if (ext == "png")		return AssetType::Image;
			if (ext == "mp3")		return AssetType::AudioClip;
			if (ext == "ogg")		return AssetType::AudioClip;
			if (ext == "hazard")	return AssetType::World;
			if (ext == "obj")		return AssetType::Mesh;
			if (ext == "fbx")		return AssetType::Mesh;
			if (ext == "dae")		return AssetType::Mesh;
			if (ext == "hdr")		return AssetType::EnvironmentMap;
			if (ext == "cs")		return AssetType::Script;

			return AssetType::Undefined;
		}
		const char* ResourceTypeToString(ResourceType type)
		{
			switch (type)
			{
			case ResourceType::VertexBuffer:	return "VertexBuffer";
			case ResourceType::IndexBuffer:		return "IndexBuffer";
			case ResourceType::UniformBuffer:	return "UniformBuffer";
			case ResourceType::Pipeline:		return "Pipeline";
			case ResourceType::Mesh:			return "Mesh";
			default:
				return "Undefined";
			}
			return "Undefined";
		}
	}
}
