#pragma once
#include <hzrpch.h>
#include "AudioSourceBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap AudioSourceBindings::GetBindings()
	{
		BindMap result;
		result.emplace_back("Hazard.AudioSource::AudioSource_Play_Native", AudioSource_Play_Native);
		result.emplace_back("Hazard.AudioSource::AudioSource_Stop_Native", AudioSource_Stop_Native);
		return result;
	}
	void AudioSourceBindings::AudioSource_Play_Native()
	{

	}
	void AudioSourceBindings::AudioSource_Stop_Native()
	{

	}
}