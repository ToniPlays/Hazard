#pragma once
#include "Core/Core.h"
#include "Asset.h"

namespace Hazard
{
	enum AssetPackFlags : uint32_t
	{
		ASSET_PACK_HEADER = BIT(0),
		ASSET_PACK_ELEMENT = BIT(1),
		ASSET_PACK_CONTAINS_DATA = BIT(2),
		ASSET_PACK_REFERENCES_FILE = BIT(3)
	};

	//Asset packs support only a single asset at the moment
	struct AssetPack
	{
		uint32_t Flags;
		uint64_t Handle;
		AssetType Type;
		std::string SourceFile;
		Buffer AssetData;

		CachedBuffer ToBuffer()
		{
			uint32_t len = SourceFile.length() + sizeof(uint64_t);
			CachedBuffer buffer(sizeof(uint32_t) + sizeof(uint64_t) + sizeof(AssetType) + len + AssetData.Size);
			buffer.Write(Flags);
			buffer.Write(Handle);
			buffer.Write(Type);
			buffer.Write(SourceFile);
			buffer.Write(AssetData);
			return buffer;
		}
		void FromBuffer(CachedBuffer buffer)
		{
			Flags = buffer.Read<uint32_t>();
			Handle = buffer.Read<uint64_t>();
			Type = buffer.Read<AssetType>();
			SourceFile = buffer.Read<std::string>();
			uint64_t dataSize = buffer.GetSize() - buffer.GetCursor();
			AssetData = Buffer::Copy(buffer.GetData(), dataSize, buffer.GetCursor());
		}
	};

}