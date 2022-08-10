#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "ScriptEngine.h"

namespace Hazard {
	class ScriptAssetLoader : public IAssetLoader {
	public:
		ScriptAssetLoader(ScriptEngine* engine) : m_Engine(engine) {};
		~ScriptAssetLoader() = default;

		bool Load(AssetMetadata& metadata, Ref<Asset>& asset);
		bool Save(Ref<Asset>& asset);

	private:
		ScriptEngine* m_Engine;
	};
}