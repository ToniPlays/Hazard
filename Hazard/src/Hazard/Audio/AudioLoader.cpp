
#include <hzrpch.h>
#include "AudioLoader.h"
#include "AudioFactory.h"

#include <thread>
#include <filesystem>

namespace Hazard::Audio 
{

	 AudioLoader::AudioLoader() {
	 	AudioFactory::Init();
	 }
	 bool AudioLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	 {
	 	asset = AudioFactory::Load(metadata.Path.string());
	 	return asset;
	 }
}
