#pragma once

#include <hzrpch.h>
#include "AudioCommand.h"
#include "Hazard/File/File.h"
#include "AudioLoader.h"

namespace Hazard::Audio 
{
	AudioClip AudioCommand::Create(ECS::AudioSourceComponent& component)
	{
		if (!File::Exists(component.sourceFile)) 
			return AudioClip();
		return AudioLoader::LoadFile(component.sourceFile);
	}
	void AudioCommand::StopAll()
	{
		//TODO: Stop all clips when needed
	}
}