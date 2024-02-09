#pragma once

#include "Hazard/Assets/Asset.h"
#include "Buffer/Buffer.h"

namespace Hazard
{
	class ShaderAsset : public Asset
	{
		friend class ShaderAssetLoader;

	public:
		ShaderAsset() = default;
		~ShaderAsset() = default;

		AssetType GetType() const override { return AssetType::Shader; }

	public:
		std::unordered_map<HazardRenderer::RenderAPI, std::unordered_map<uint32_t, std::string>> ShaderCode;
	};
}