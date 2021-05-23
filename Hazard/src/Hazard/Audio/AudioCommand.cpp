#pragma once

#include <hzrpch.h>
#include "AudioCommand.h"
#include "Hazard/File/File.h"
#include "AudioLoader.h"

namespace Hazard::Audio 
{
	AudioClip AudioCommand::Create(ECS::AudioSourceComponent& component)
	{
		HZR_CORE_INFO("Creating source for {0}", component.sourceFile);
		if (File::Exists(component.sourceFile.c_str())) {
			return AudioLoader::LoadFile(component.sourceFile);
		}
		return AudioClip();
	}
	void AudioCommand::StopAll()
	{

	}
}