#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard 
{
	class WorldSerializer;
	class WorldDeserializer;
	class World;

	class WorldAssetLoader : public IAssetLoader 
	{
	public:
		WorldAssetLoader() = default;
		~WorldAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

	private:
		static void GetWorldContent(JobInfo& info, WorldSerializer serializer, uint32_t assetSaveFlags);

		static void PreprocessWorldFile(JobInfo& info, AssetHandle handle);
		static void LoadRequiredAsset(JobInfo& info, AssetHandle handle);
		static void FinalizeWorld(JobInfo& info, AssetHandle handle);

		static void CreateWorld(JobInfo& info, const std::filesystem::path& file);
	};

	struct WorldAssetHeader
	{
		std::string SourcePath;

		CachedBuffer ToBuffer()
		{
			CachedBuffer buf(SourcePath.length() + sizeof(uint64_t));
			buf.Write(SourcePath);
			return buf;
		}
		void FromBuffer(CachedBuffer buffer)
		{
			CachedBuffer buf(buffer);
			SourcePath = buf.Read<std::string>();
		}
	};
}
