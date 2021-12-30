#pragma once

#include <hzrpch.h>
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

	namespace Utils 
	{
		inline AssetType StringToAssetType(const std::string& type) {
			if (type == "None")					return AssetType::Undefined;
			if (type == "AudioClip")			return AssetType::AudioClip;
			if (type == "World")				return AssetType::World;
			if (type == "Texture")				return AssetType::Image;
			if (type == "PhysicsMaterial")		return AssetType::PhysicsMaterial;
			if (type == "Mesh")					return AssetType::Mesh;
			if (type == "EnvironmentMap")		return AssetType::EnvironmentMap;
			if (type == "Script")				return AssetType::Script;

			HZR_CORE_WARN("Failed to find asset of type {0}, check if type is correct", type);

			return AssetType::Undefined;
		}

		inline const char* AssetTypeToString(AssetType type) {
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
			}
		}

		inline AssetType AssetTypeFromExtension(const std::string& ext) {
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
			
			HZR_CORE_WARN("Failed convert extesion {0} to AssetType, check if type is correct", ext);
			return AssetType::Undefined;
		}
	}
}