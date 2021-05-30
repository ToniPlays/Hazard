#pragma once

#include <hzrpch.h>
#include "Vault.h"

namespace Hazard 
{
	std::unordered_map<std::string, Rendering::Shader*> Vault::s_Shaders;
	std::unordered_map<std::string, Rendering::Texture*> Vault::s_Textures;
	std::vector<Rendering::GraphicsPipeline*> Vault::s_Pipelines;
	std::unordered_map<std::string, Audio::AudioBufferData*> Vault::s_Audio;
}