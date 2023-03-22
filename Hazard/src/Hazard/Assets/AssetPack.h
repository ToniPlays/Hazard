#pragma once

#include "Asset.h"

namespace Hazard
{
	struct AssetPackHeader
	{
		uint32_t ElementCount = 0;
	};

	struct AssetPackElementHeader
	{
		uint64_t Handle;
		uint32_t Type;
		size_t AssetDataSize = 0;
		uint32_t AssetDataOffset = 0;
	};
	struct AssetPackElement
	{
		std::filesystem::path AssetPack;
		AssetHandle Handle;
		AssetType Type;
		Buffer Data;
	};

	struct AssetPack
	{
		uint32_t ElementCount = 0;
		std::vector<AssetPackElement> Elements;

		void Free()
		{
			for (auto& element : Elements)
				element.Data.Release();
		}

		static AssetPack Create(CachedBuffer& buffer, const std::filesystem::path& path)
		{
			buffer.ResetCursor();
			
			AssetPack pack;
			AssetPackHeader header = buffer.Read<AssetPackHeader>();

			pack.ElementCount = header.ElementCount;
			AssetPackElementHeader* headers = (AssetPackElementHeader*)buffer.Read<Buffer>(sizeof(AssetPackElementHeader) * pack.ElementCount).Data;

			pack.ElementCount = header.ElementCount;
			pack.Elements.reserve(header.ElementCount);
			
			for (uint32_t i = 0; i < header.ElementCount; i++)
			{
				AssetPackElement& element = pack.Elements.emplace_back();

				element.AssetPack = path;
				element.Type = (AssetType)headers[i].Type;
				element.Handle = headers[i].Handle;
				element.Data = buffer.Read<Buffer>(headers[i].AssetDataSize);
			}
			
			return pack;
		}
	};
}