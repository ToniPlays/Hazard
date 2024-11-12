#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Buffer/CachedBuffer.h"

namespace Hazard 
{
	class WorldSerializer;
	class WorldDeserializer;
	class World;


	class WorldAssetLoader : public IAssetLoader 
	{
	public:
		struct CreateSettings
        {
			
        };

		WorldAssetLoader() = default;
		~WorldAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata, const LoadAssetSettings& settings) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

	private:
		static Coroutine GetWorldContent(JobInfo& info, WorldSerializer serializer, uint32_t assetSaveFlags);

		static Coroutine PreprocessWorldFile(JobInfo& info, AssetHandle handle, const LoadAssetSettings& settings);
		static Coroutine LoadRequiredAsset(JobInfo& info, AssetHandle handle);
		static Coroutine FinalizeWorld(JobInfo& info, AssetHandle handle);

		static Coroutine CreateWorld(JobInfo& info, const std::filesystem::path& file);
	};

	struct WorldAssetHeader
	{
		std::string SourcePath;

		Ref<CachedBuffer> ToBuffer()
		{
			Ref<CachedBuffer> buf = Ref<CachedBuffer>::Create(SourcePath.length() + sizeof(uint64_t));
			buf->Write(SourcePath);
			return buf;
		}
		void FromBuffer(Ref<CachedBuffer> buffer)
		{
			SourcePath = buffer->Read<std::string>();
		}
	};
}
