#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "ScriptEngine.h"

namespace Hazard 
{
	class ScriptAssetLoader : public IAssetLoader 
	{
	public:
		ScriptAssetLoader(ScriptEngine* engine) : m_Engine(engine) {};
		~ScriptAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		Ref<JobGraph> DataFromSource(const std::filesystem::path& path) override { return nullptr; };
		Ref<JobGraph> Create(const std::filesystem::path& path, const std::filesystem::path& internalPath) override;

		Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };

	private:
		ScriptEngine* m_Engine;
	};
}
