#pragma once

#include "hzrpch.h"
#include "Hazard/Assets/AssetEnums.h"
#include "Hazard/Assets/Asset.h"
#include <Severity.h>
#include "UtilityCore.h"

namespace Exporter
{

	struct BuildMessage
	{
		std::string Message;
		std::string Context;
		Severity MessageSeverity;

		BuildMessage(std::string message, std::string context, Severity severity) : Message(message), Context(context), MessageSeverity(severity)
		{
		}
	};
	struct BuildResource
	{
		AssetType Type;
		std::string FileName;
		uint64_t Size;

		BuildResource() = default;
		BuildResource(AssetType type, const std::string& file, uint64_t size) : Type(type), FileName(file), Size(size)
		{
		}
	};

	struct BuildReport
	{
		uint64_t BuildTime = 0;
		uint64_t TotalSize = 0;
		bool Result = false;

		std::vector<BuildMessage> Messages;
		std::unordered_map<AssetType, std::vector<BuildResource>> Resources;
		std::unordered_map<AssetType, uint64_t> AssetTypeSize;

		BuildReport() = default;

		void AddMessage(BuildMessage message)
		{
			Messages.push_back(message);
		}
		void AddResource(BuildResource resource)
		{
			Resources[resource.Type].push_back(resource);
			TotalSize += resource.Size;
			AssetTypeSize[resource.Type] += resource.Size;
		}
	};
}
