#pragma once

#include <hzrpch.h>
#include "AudioCommand.h"
#include "AudioLoader.h"

namespace Hazard::Audio 
{
	Ref<AudioClip> AudioCommand::Create(AudioClipCreateInfo* createInfo)
	{
		HZR_CORE_INFO("[Audio]: Loading file {0}", createInfo->FileName);
		return nullptr;
	}
	void AudioCommand::StopAll()
	{
		
	}
}