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

		Ref<JobGraph> Load(AssetMetadata& metadata, Ref<Asset>& asset) override;
		//Ref<JobGraph> LoadAsync(AssetMetadata& metadata, uint32_t flags) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		//Ref<JobGraph> SaveAsync(Ref<Asset>& asset) override;

	private:
		ScriptEngine* m_Engine;
	};
}