
#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
	class ShaderAssetLoader : public IAssetLoader
	{
	public:
		ShaderAssetLoader() = default;
		~ShaderAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags = 0) override;
		Ref<JobGraph> LoadAsync(AssetMetadata& metadata, uint32_t flags = 0) override;
		bool Save(Ref<Asset>& asset) override;
		Ref<JobGraph> SaveAsync(Ref<Asset>& asset) override;
	};
}