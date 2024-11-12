
#include "Hazard/Assets/IAssetLoader.h"
#include "Core/RenderContextCreateInfo.h"
#include "Utility/Coroutine.h"


namespace Hazard
{
	struct ShaderAPIHeader
	{
		uint32_t ApiFlags;	
		uint32_t StageFlags;
	};

	class ShaderAsset;

	class ShaderAssetLoader : public IAssetLoader
	{
	public:
		ShaderAssetLoader() = default;
		~ShaderAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata, const LoadAssetSettings& settings) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

	private:
		static Coroutine LoadShaderSource(JobInfo& info, const std::filesystem::path& path);
		static Coroutine CompileShaderSourceCode(JobInfo& info, uint32_t api, uint32_t stageFlags);
		static Coroutine CreateShaderAsset(JobInfo& info);

		static Coroutine GenerateShaderAssetBinary(JobInfo& info, Ref<ShaderAsset> asset);
		static Coroutine LoadShaderAsset(JobInfo& info, AssetHandle handle);

		struct ShaderCompileResult
		{
			uint32_t API;
			std::string Data;
			uint32_t Flags;
		};
	};
}
