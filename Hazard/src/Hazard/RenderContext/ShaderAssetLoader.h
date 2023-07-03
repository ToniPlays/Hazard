
#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard
{
	class ShaderAssetLoader : public IAssetLoader
	{
	public:
		ShaderAssetLoader() = default;
		~ShaderAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		Ref<JobGraph> Create(const std::filesystem::path& path) override;
	};
}