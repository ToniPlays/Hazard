#pragma once

#include "Asset.h"

namespace Hazard
{
	struct AssetPackHeader
	{
		AssetHandle Handle = INVALID_ASSET_HANDLE;
		uint64_t ElementCount = 0;
	};

	struct AssetPackElementHeader
	{
		uint64_t Handle;
		uint64_t AssetDataSize = 0;
		uint64_t AssetDataOffset = 0;
		uint32_t Type;
		//Store addressable name here
	};
	struct AssetPackElement
	{
		AssetHandle AssetPackHandle;
		AssetHandle Handle;
		AssetType Type;
		Buffer Data;
		std::string AddressableName;
	};

	struct AssetPack
	{
		AssetHandle Handle;
		uint64_t ElementCount = 0;
		std::vector<AssetPackElement> Elements;

		void Free()
		{
			for (auto& element : Elements)
				element.Data.Release();
		}

		static AssetPack Create(CachedBuffer& buffer)
		{
			buffer.ResetCursor();
			
			AssetPack pack;
			AssetPackHeader header = buffer.Read<AssetPackHeader>();

			pack.Handle = header.Handle;
			pack.ElementCount = header.ElementCount;
			pack.Elements.resize(header.ElementCount);

			std::vector<AssetPackElementHeader> headers;
			headers.resize(header.ElementCount);

			for (uint64_t i = 0; i < header.ElementCount; i++)
			{
				headers[i] = buffer.Read<AssetPackElementHeader>();
				AssetPackElement& element = pack.Elements[i];
				element.Handle = headers[i].Handle;
				element.Type = (AssetType)headers[i].Type;
				element.AddressableName = buffer.Read<std::string>();
			}
			
			for (uint64_t i = 0; i < header.ElementCount; i++)
			{
				AssetPackElement& element = pack.Elements[i];
				element.AssetPackHandle = pack.Handle;
				element.Data = buffer.Read<Buffer>(headers[i].AssetDataSize);
			}
			
			return pack;
		}
		static CachedBuffer ToBuffer(const AssetPack& pack)
		{
			AssetPackHeader header = {};
			header.Handle = pack.Handle;
			header.ElementCount = pack.ElementCount;

			uint64_t dataSize = 0;

			for (auto& element : pack.Elements)
				dataSize += element.Data.Size + sizeof(uint64_t) + element.AddressableName.length() * sizeof(char);

			CachedBuffer buffer(sizeof(AssetPackHeader) + sizeof(AssetPackElementHeader) * pack.ElementCount + dataSize);

			uint64_t dataOffset = 0;
			buffer.Write(header);

			//Write headers
			for (auto& element : pack.Elements)
			{
				AssetPackElementHeader elementHeader = {};
				elementHeader.Type = (uint32_t)element.Type;
				elementHeader.AssetDataSize = element.Data.Size;
				elementHeader.AssetDataOffset = dataOffset;
				elementHeader.Handle = element.Handle;
				buffer.Write(elementHeader);
				buffer.Write<std::string>(element.AddressableName);

				dataOffset += element.Data.Size;
			}
			//Write actual asset data
			for (auto& element : pack.Elements)
				buffer.Write(element.Data);

			return buffer;
		}
	};
}