#pragma once

#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard::Rendering {
	class TextureLoader : public IAssetLoader {
	public:
		virtual bool Load(AssetMetadata& metadata, Ref<Asset>& asset);
		virtual bool Save(Ref<Asset>& asset) { return false; };
	};
}