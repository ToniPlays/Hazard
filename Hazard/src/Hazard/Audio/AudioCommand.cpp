#pragma once

#include <hzrpch.h>
#include "AudioCommand.h"
#include "AudioLoader.h"

namespace Hazard::Audio 
{
	AudioClip AudioCommand::Create(ECS::AudioSourceComponent& component)
	{
		if (!File::Exists(component.SourceFile)) 
			return AudioClip();
		return AudioLoader::LoadFile(component.SourceFile);
	}
	void AudioCommand::StopAll()
	{
		//TODO: Stop all clips when needed
	}
}