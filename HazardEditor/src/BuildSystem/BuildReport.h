#pragma once

#include "Hazard.h"

namespace Exporter {
	enum class BuildMessageSeverity 
	{
		Info = 0,
		Warning,
		Error,
		Fatal
	};

	struct BuildMessage {
		std::string Message;
		std::string Context;
		BuildMessageSeverity Severity;

		BuildMessage(std::string message, std::string context, BuildMessageSeverity severity) : Message(message), Context(context), Severity(severity) {}
	};
	struct BuildResource {
		Hazard::AssetType Type;
		std::string FileName;
		size_t Size;

		BuildResource() = default;
		BuildResource(Hazard::AssetType type, const std::string& file, size_t size) : Type(type), FileName(file), Size(size) {}
	};

	struct BuildReport
	{
		size_t BuildTime;
		size_t TotalSize;
		bool Result;

		std::vector<BuildMessage> Messages;
		std::unordered_map<Hazard::AssetType, std::vector<BuildResource>> Resources;
		std::unordered_map<Hazard::AssetType, size_t> AssetTypeSize;

		BuildReport() 
		{
			AssetTypeSize[Hazard::AssetType::World] = 0;
			AssetTypeSize[Hazard::AssetType::Image] = 0;
			AssetTypeSize[Hazard::AssetType::Mesh] = 0;
			AssetTypeSize[Hazard::AssetType::EnvironmentMap] = 0;
			AssetTypeSize[Hazard::AssetType::PhysicsMaterial] = 0;
			AssetTypeSize[Hazard::AssetType::AudioClip] = 0;
		}

		void AddMessage(BuildMessage message) { Messages.push_back(message); }
		void AddResource(BuildResource resource) { 
			Resources[resource.Type].push_back(resource); 
			TotalSize += resource.Size;
			AssetTypeSize[resource.Type] += resource.Size;
		}
	};
}