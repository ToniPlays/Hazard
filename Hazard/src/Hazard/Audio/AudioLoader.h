#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Assets/IAssetLoader.h"

namespace Hazard::Audio {

	class AudioClip;
	enum class FileFormat {
		None = 0,
		Ogg,
		Mp3,
		Waw
	};

	struct AudioClipCreateInfo 
	{
		std::string FileName;
		float Gain = 1.0f;
		float Pitch = 1.0f;
		bool PlayOnCreate = false;
		bool Looping = false;
		bool Spatial = false;
	};


	class AudioLoader : public IAssetLoader {
	public:

		AudioLoader();
		virtual bool Load(AssetMetadata& metadata, Ref<Asset>& asset);
		virtual bool Save(Ref<Asset>& asset) { return false; };
	};
}