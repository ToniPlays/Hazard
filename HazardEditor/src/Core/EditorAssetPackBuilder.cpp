#include "EditorAssetPackBuilder.h"

AssetPack EditorAssetPackBuilder::CreateAssetPack(const std::vector<AssetPackElement> element)
{
    AssetPack pack = {};
    pack.ElementCount = element.size();
    pack.Elements = element;

    return pack;
}
CachedBuffer EditorAssetPackBuilder::AssetPackToBuffer(const AssetPack& pack)
{
	AssetPackHeader header = {};
	header.ElementCount = pack.ElementCount;

	size_t dataSize = 0;

	for (auto& element : pack.Elements)
		dataSize += element.Data.Size;

	CachedBuffer buffer(sizeof(AssetPackHeader) + sizeof(AssetPackElementHeader) * pack.ElementCount + dataSize);

	uint32_t dataOffset = 0;

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

		dataOffset += element.Data.Size;
	}
	//Write actual asset data
	for (auto& element : pack.Elements)
		buffer.Write(element.Data);

	return buffer;
}