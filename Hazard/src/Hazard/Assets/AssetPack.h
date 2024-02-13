#pragma once
#include "Core/Core.h"
#include "Asset.h"

#include "Buffer/CachedBuffer.h"

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
		Ref<CachedBuffer> AssetData;

		Ref<CachedBuffer> ToBuffer()
		{
			uint32_t len = SourceFile.length() + sizeof(uint64_t) + (AssetData ? AssetData->GetSize() : 0);
			Ref<CachedBuffer> buffer = Ref<CachedBuffer>::Create(sizeof(uint32_t) + sizeof(uint64_t) + sizeof(AssetType) + len);

			buffer->Write(Flags);
			buffer->Write(Handle);
			buffer->Write(Type);
			buffer->Write(SourceFile);
			if (AssetData)
				buffer->Write(AssetData->GetData(), AssetData->GetSize());

			return buffer;
		}
		void FromBuffer(Ref<CachedBuffer> buffer)
		{
			Flags = buffer->Read<uint32_t>();
			Handle = buffer->Read<uint64_t>();
			Type = buffer->Read<AssetType>();
			SourceFile = buffer->Read<std::string>();
			uint64_t dataSize = buffer->GetSize() - buffer->GetCursor();
			AssetData = Ref<CachedBuffer>::Create(Buffer::Copy(buffer->GetData(), dataSize, buffer->GetCursor()));
		}
	};

}