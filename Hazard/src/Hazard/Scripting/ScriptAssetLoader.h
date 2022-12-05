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

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags) override;
		TypedJobPromise<Ref<Asset>> LoadAsync(AssetMetadata& metadata, uint32_t flags) override;
		bool Save(Ref<Asset>& asset) override;
		JobPromise SaveAsync(Ref<Asset>& asset) override;

	private:
		ScriptEngine* m_Engine;
	};
}