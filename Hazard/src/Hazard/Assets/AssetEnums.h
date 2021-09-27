#pragma once

#include <hzrpch.h>
#include "Hazard/Core/Core.h"

namespace Hazard {
	enum class AssetType {
		None = 0,
		AudioClip,
		World,
		Texture,
		Mesh,
		EnvironmentMap,
		PhysicsMaterial
	};

	namespace Utils {
		inline AssetType StringToAssetType(const std::string& type) {
			if (type == "None")					return AssetType::None;
			if (type == "AudioClip")			return AssetType::AudioClip;
			if (type == "World")				return AssetType::World;
			if (type == "Texture")				return AssetType::Texture;
			if (type == "PhysicsMaterial")		return AssetType::PhysicsMaterial;
			if (type == "Mesh")					return AssetType::Mesh;

			HZR_CORE_WARN("Failed to find asset of type {0}, check if type is correct", type);

			return AssetType::None;
		}
		inline const char* AssetTypeToString(AssetType type) {
			switch (type)
			{
			case AssetType::None:				return "None";
			case AssetType::AudioClip:			return "AudioClip";
			case AssetType::World:				return "World";
			case AssetType::Texture:			return "Texture";
			case AssetType::PhysicsMaterial:	return "PhysicsMaterial";
			case AssetType::Mesh:				return "Mesh";
			}
		}
		inline AssetType AssetTypeFromExtension(const std::string& ext) {
			if (ext == "jpeg")		return AssetType::Texture;
			if (ext == "jpg")		return AssetType::Texture;
			if (ext == "png")		return AssetType::Texture;
			if (ext == "mp3")		return AssetType::AudioClip;
			if (ext == "ogg")		return AssetType::AudioClip;
			if (ext == "hazard")	return AssetType::World;
			if (ext == "obj")		return AssetType::Mesh;
			if (ext == "fbk")		return AssetType::Mesh;
			
			HZR_CORE_WARN("Failed convert extesion {0} to AssetType, check if type is correct", ext);
			return AssetType::None;
		}
	}
}