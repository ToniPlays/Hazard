
#include "Hazard/Assets/IAssetLoader.h"

#include "Core/RenderContextCreateInfo.h"

namespace Hazard
{
	struct ShaderAssetDataHeader
	{
		uint16_t IncludedAPIs = 0;
		uint16_t Stages = 0;
	};
	struct ShaderAPIDataHeader
	{
		uint32_t DataSize = 0;
	};

	class ShaderAssetLoader : public IAssetLoader
	{
	public:
		ShaderAssetLoader() = default;
		~ShaderAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		Ref<JobGraph> DataFromSource(const std::filesystem::path& path) override;
		Ref<JobGraph> Create(const std::filesystem::path& path) override;

		Buffer ToBinary(Ref<Asset> asset) override;

	private:
		static void LoadShader(JobInfo& info, AssetHandle handle);
		static void LoadSourceCode(JobInfo& info, const std::string& source, uint32_t stage, HazardRenderer::RenderAPI api);
		static void CreateShaderAsset(JobInfo& info, const std::filesystem::path& path);
	};
}